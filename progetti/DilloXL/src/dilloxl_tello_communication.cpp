/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) 2024 Copyright by Michele Iacobellis
 * A project for students...
 * 
 * This file is part of DILLOXL.
 *
 * DILLOXL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DILLOXL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DILLOXL. If not, see <http://www.gnu.org/licenses/>.
 * 
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_tello_communication.h"
#include "dilloxl_tello_drone.h"
#include <queue>
#include <thread>
#include <mutex>
#include <memory.h>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * MACROS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define DILLOXL_TELLOCOMM_DUMP_DEBUG                                       0
#define DILLOXL_TELLOCOMM_QUEUE_DEBUG                                      1

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * MACROS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define DILLOXL_TELLOCOMM_LINK_DOWN_TIMEOUT_S                              2
#define DILLOXL_TELLOCOMM_WAITFORRESP_TIMEOUT_S                           15
#define DILLOXL_TELLOCOMM_RES_PRINT                                        1

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATIONS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTIONS
 * Pay attention, the "land" command may require many seconds before having
 * an "ok" response from the drone (other commands may too).
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void DroneWaitAndSend(const std::function<void()>& fn) {
  if (dilloxl::TelloDrone::Get().com().waitforready(200 /* units of 50ms each */)) {
    if (fn) { fn(); } // send is thread-safe now
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::TelloCommunication::Impl {
public:
  Impl(TelloCommunication* pO)
  : m_pO                  {                 pO }
  , m_st                  { Status::kUNDEFINED }
  , m_szNControlPacketsIn {                  0 }
  , m_szNControlPacketsOt {                  0 }
  , m_szNStatusPackets    {                  0 }
  , m_szNVideoPackets     {                  0 }
  , m_szNStatusPacketsLast{                  0 }
  , m_bLinkAlive          {              false }
  , m_bWaitForResponse    {              false }
  , m_bControlPktSent     {              false }
  , m_uSeqToWaitFor       {                  0 }
  {
    ::memset(m_strSerial, 0, sizeof(m_strSerial));
    m_strSerial[0] = '?';
  }

 ~Impl() {

  }

  TelloCommunication* m_pO;
  sf::UdpSocket m_SockControl;
  sf::UdpSocket m_SockStatus;
  sf::UdpSocket m_SockVideo;
  Status m_st;
  std::string m_strLastErr;
  size_t m_szNControlPacketsIn;
  size_t m_szNControlPacketsOt;
  size_t m_szNStatusPackets;
  size_t m_szNVideoPackets;
  size_t m_szNStatusPacketsLast;

  ContrlCallback m_contrlcb;
  StatusCallback m_statuscb;
  VideoSCallback m_videoscb;
  bool m_bLinkAlive;
  bool m_bWaitForResponse;
  bool m_bControlPktSent;
  std::recursive_mutex m_mtxW4RCPS;
  Timer m_timerKeepAlive;
  Timer m_timerW4R;
  std::queue<std::string> m_qMsg;
  uint32_t m_uSeqToWaitFor;
  std::recursive_mutex m_mtxQ;
  char m_strSerial[64];

  void m_OnStatusUnconnected();
  void m_OnStatusConnecting(); 
  void m_OnStatusConnected();  
  void m_OnStatusWorking();    
  void m_OnStatusGetInfo();  
  void m_OnStatusError();      
  void m_OnStatusUndefined();
  void m_DequeueAndSend();
  void m_Send(const std::string&);
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::TelloCommunication::StatusToString(Status st)
{
  switch (st) {
  case Status::kUNDEFINED  : return "NON DEFINITO";
  case Status::kUNCONNECTED: return "NON CONNESSO";
  case Status::kCONNECTING : return "IN CONNESSIONE";
  case Status::kCONNECTED  : return "CONNESSO";
  case Status::kWORKING    : return "A LAVORO";
  case Status::kGETINFO    : return "RACCOLTA INFO";
  case Status::kERROR      : return "IN ERRORE";
  default: break;
  }
  return "INATTESO";
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloCommunication::TelloCommunication()
: m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{ this };
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Allocazione fallita");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloCommunication::~TelloCommunication()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::reset()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  std::lock_guard<std::recursive_mutex> g{ m_pImpl->m_mtxW4RCPS };
  m_pImpl->m_strLastErr = "Nessuno";
  m_pImpl->m_szNControlPacketsIn  =     0;
  m_pImpl->m_szNControlPacketsOt  =     0;
  m_pImpl->m_szNStatusPackets     =     0;
  m_pImpl->m_szNVideoPackets      =     0;
  m_pImpl->m_szNStatusPacketsLast =     0;
  m_pImpl->m_bLinkAlive           = false;
  m_pImpl->m_bWaitForResponse     = false;
  m_pImpl->m_bControlPktSent      = false;
  m_pImpl->m_uSeqToWaitFor        =     0;
  while (!m_pImpl->m_qMsg.empty()) { m_pImpl->m_qMsg.pop(); }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::tryLink()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  std::lock_guard<std::recursive_mutex> g{ m_pImpl->m_mtxW4RCPS };
  m_pImpl->m_strLastErr = "Nessuno";
  m_pImpl->m_szNControlPacketsIn  =     0;
  m_pImpl->m_szNControlPacketsOt  =     0;
  m_pImpl->m_szNStatusPackets     =     0;
  m_pImpl->m_szNVideoPackets      =     0;
  m_pImpl->m_szNStatusPacketsLast =     0;
  m_pImpl->m_bLinkAlive           = false;
  m_pImpl->m_bWaitForResponse     = false;
  m_pImpl->m_bControlPktSent      = false;
  m_pImpl->m_uSeqToWaitFor        =     0;
  while (!m_pImpl->m_qMsg.empty()) { m_pImpl->m_qMsg.pop(); }

  m_pImpl->m_SockControl.unbind();
   m_pImpl->m_SockStatus.unbind();
    m_pImpl->m_SockVideo.unbind();
  m_pImpl->m_st = Status::kUNCONNECTED;
  if (m_pImpl->m_SockControl.bind(kUDP_PORT_CONTROL) != sf::Socket::Status::Done) {
    m_pImpl->m_strLastErr = "Problemi sul socket di controllo";
    m_pImpl->m_st = Status::kERROR;
    return;
  }
  if (m_pImpl->m_SockStatus.bind(kUDP_PORT_STATUS) != sf::Socket::Status::Done) {
    m_pImpl->m_strLastErr = "Problemi sul socket di stato";
    m_pImpl->m_st = Status::kERROR;
    return;
  }
  if (m_pImpl->m_SockVideo.bind(kUDP_PORT_VIDEO) != sf::Socket::Status::Done) {
    m_pImpl->m_strLastErr = "Problemi sul socket video";
    m_pImpl->m_st = Status::kERROR;
    return;
  }
  m_pImpl->m_SockControl.setBlocking(false);
   m_pImpl->m_SockStatus.setBlocking(false);
    m_pImpl->m_SockVideo.setBlocking(false);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::execute()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  switch (m_pImpl->m_st) {
  case Status::kUNCONNECTED: m_pImpl->m_OnStatusUnconnected(); break;
  case  Status::kCONNECTING: m_pImpl->m_OnStatusConnecting();  break;
  case   Status::kCONNECTED: m_pImpl->m_OnStatusConnected();   break;
  case     Status::kWORKING: m_pImpl->m_OnStatusWorking();     break;
  case     Status::kGETINFO: m_pImpl->m_OnStatusGetInfo();     break;
  case       Status::kERROR: m_pImpl->m_OnStatusError();       break;
  default:                   m_pImpl->m_OnStatusUndefined();   break;
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
const std::string& dilloxl::TelloCommunication::lastError() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_strLastErr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::TelloCommunication::lastStatus() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return StatusToString(m_pImpl->m_st);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD (The program thread will wait here)
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::TelloCommunication::waitforready(size_t timeout_50ms)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  for (size_t k = 0;k < timeout_50ms; ++k) {
    bool bCPS = false, bW4R = false;
    { std::lock_guard<std::recursive_mutex> g{ m_pImpl->m_mtxW4RCPS };
      bCPS = m_pImpl->m_bControlPktSent; bW4R = m_pImpl->m_bWaitForResponse; 
    }
    if (bCPS && bW4R) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    } else {
      std::fprintf(stderr
        , DILLOXL_TERM_FGDIMM 
          "[COM]: RISPOSTA (CPS=%s W4R=%s)." 
          DILLOXL_TERM_RESETA "\n"
        ,  m_pImpl->m_bControlPktSent ? "YES" : "NO"
        , m_pImpl->m_bWaitForResponse ? "YES" : "NO");
      return true;
    }
  }
  std::fprintf(stderr
    , DILLOXL_TERM_YLWBLK 
      "[COM]: TIMEOUT (%zu x50ms)." 
      DILLOXL_TERM_RESETA "\n"
    , timeout_50ms);
  return false; // timeout
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::send(const std::string& msg
  , SendMode mode)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  if (SendMode::kFORCE == mode) {
    std::fprintf(stderr
      , DILLOXL_TERM_GRYBLK
        "[COM]: MESSAGGIO URGENTE \"%s\"..." 
        DILLOXL_TERM_RESETA "\n"
      , msg.c_str());
    m_pImpl->m_Send(msg);
  } else {
    std::lock_guard<std::recursive_mutex> g{ m_pImpl->m_mtxW4RCPS };
    if (m_pImpl->m_bWaitForResponse && m_pImpl->m_bControlPktSent) {
      std::lock_guard<std::recursive_mutex> g{ m_pImpl->m_mtxQ };
      m_pImpl->m_qMsg.push(msg);
      std::fprintf(stderr
        , DILLOXL_TERM_GRYBLK 
          "[COM]: Messaggio \"%s\" accodato." 
          DILLOXL_TERM_RESETA "\n"
        , msg.c_str());
    } else {
      m_pImpl->m_Send(msg);
    }
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::setContrlCallback(const ContrlCallback& cb)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_contrlcb = cb;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::setStatusCallback(const StatusCallback& cb)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_statuscb = cb;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::setVideosCallback(const VideoSCallback& cb)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_videoscb = cb;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::TelloCommunication::isLinkAlive() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_bLinkAlive;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::TelloCommunication::isWaitingForResponse() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_bWaitForResponse;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
size_t dilloxl::TelloCommunication::nCtrlPktsIn() const 
{ 
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_szNControlPacketsIn; 
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
size_t dilloxl::TelloCommunication::nCtrlPktsOut() const 
{ 
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_szNControlPacketsOt; 
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
size_t dilloxl::TelloCommunication::nStatusPkts() const 
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_szNStatusPackets; 
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
size_t dilloxl::TelloCommunication::nVideoPkts() const 
{ 
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_szNVideoPackets;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::TelloCommunication::serial() const 
{ 
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_strSerial;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusUnconnected()
{
  // send activation command...
  std::string msg{ "command" };
  auto to = sf::IpAddress::resolve(kTELLO_IPv4);
  if (m_SockControl.send(msg.c_str(), msg.size() + 1 // notice the +1 here
    , to.value(), kUDP_PORT_CONTROL) != sf::Socket::Status::Done) 
  {
    m_strLastErr = "Invio messaggio attivazione fallito";
    m_st = Status::kERROR;
  } else {
    m_strLastErr = "Nessuno";
    m_st = Status::kCONNECTING;
    m_szNControlPacketsOt++;
    m_bControlPktSent  = true;
    m_bWaitForResponse = true;
    m_uSeqToWaitFor++;
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusConnecting()
{
  uint8_t btData[128];
  size_t szInBytes = 0;

  // ...wait for response to activation command
  auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
  auto ret = m_SockControl.receive(btData, sizeof(btData)
    , szInBytes, from, u2p);
  switch (ret) {
  case sf::Socket::Status::Done:
    m_strLastErr = "Nessuno";
    m_st = Status::kCONNECTED;
    m_szNControlPacketsIn++;
    m_bWaitForResponse = false;
    m_bControlPktSent  = false;
    m_uSeqToWaitFor    = 0;
#if DILLOXL_TELLOCOMM_DUMP_DEBUG == 1
    dump_data("CTRL", btData, szInBytes);
#endif    
    break;
  case sf::Socket::Status::NotReady:
    break;
  default:
    m_strLastErr = "Errore ricezione risposta ad attivazione";
    m_st = Status::kERROR;
    break;
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusConnected() 
{
  // send get-serial command...
  std::string msg{ "sn?" };
  auto to = sf::IpAddress::resolve(kTELLO_IPv4);
  if (m_SockControl.send(msg.c_str(), msg.size() + 1 // notice the +1 here
    , to.value(), kUDP_PORT_CONTROL) != sf::Socket::Status::Done) 
  {
    m_strLastErr = "Invio messaggio informativo fallito";
    m_st = Status::kERROR;
  } else {
    m_strLastErr = "Nessuno";
    m_st = Status::kGETINFO;
    m_szNControlPacketsOt++;
    m_bControlPktSent  = true;
    m_bWaitForResponse = true;
    m_uSeqToWaitFor++;
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusWorking()  
{
  static uint8_t btData[8192];
  size_t szInBytes = 0;

  // check and dequeue queued messages
  if (!m_bWaitForResponse && !m_bControlPktSent) { m_DequeueAndSend(); }

  // ...wait for control packets
  { auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
    auto ret = m_SockControl.receive(btData, sizeof(btData)
      , szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done: {
  #if DILLOXL_TELLOCOMM_DUMP_DEBUG == 1
        dump_data("CONTROL", btData, szInBytes);
  #endif
      std::lock_guard<std::recursive_mutex> g{ m_mtxW4RCPS };
      if (m_bWaitForResponse && m_bControlPktSent) {
        m_strLastErr = "Nessuno";
        btData[std::min(sizeof(btData) - 1, szInBytes)] = 0;
#if DILLOXL_TELLOCOMM_RES_PRINT == 1
      auto res = trim(reinterpret_cast<const char*>(btData));
      if ("ok" == res) {
        std::fprintf(stderr
          , DILLOXL_TERM_GRYGRN 
            "[COM]: RISPOSTA RICEVUTA PER SEQ=%u, RES=%s" 
            DILLOXL_TERM_RESETA "\n"
          , m_uSeqToWaitFor, res.c_str());
      } else {
        std::fprintf(stderr
          , DILLOXL_TERM_GRYRED
            "[COM]: RISPOSTA RICEVUTA PER SEQ=%u, RES=%s" 
            DILLOXL_TERM_RESETA "\n"
          , m_uSeqToWaitFor, res.c_str());
      }
#endif        
      } else {
        m_strLastErr = "Pacchetto di controllo inatteso";
      }
      m_st = Status::kWORKING;
      m_szNControlPacketsIn++;
      m_bWaitForResponse = false;
      m_bControlPktSent  = false;
      if (m_contrlcb) { m_contrlcb(btData, szInBytes); }
    }
      break;
    case sf::Socket::Status::NotReady:
      break;
    default:
      m_strLastErr = "Errore ricezione pacchetto di controllo (res)";
      m_st = Status::kERROR;
      break;
    }
  }

  // ...wait for status packets (use this for determining alive)
  { auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
    auto ret = m_SockStatus.receive(btData, sizeof(btData)
      , szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done:
      m_strLastErr = "Nessuno";
      m_st = Status::kWORKING;
      m_szNStatusPackets++;
#if DILLOXL_TELLOCOMM_DUMP_DEBUG == 1
      dump_data("STATUS", btData, szInBytes);
#endif
      if (m_statuscb) { m_statuscb(btData, szInBytes); }
      break;
    case sf::Socket::Status::NotReady:
      break;
    default:
      m_strLastErr = "Errore ricezione pacchetto di stato";
      m_st = Status::kERROR;
      break;
    }
  }

  // ...wait for video packets
  { auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
    auto ret = m_SockVideo.receive(btData, sizeof(btData)
      , szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done:
      m_strLastErr = "Nessuno";
      m_st = Status::kWORKING;
      m_szNVideoPackets++;
      if (m_videoscb) { m_videoscb(btData, szInBytes); }
      break;
    case sf::Socket::Status::NotReady:
      break;
    default:
      m_strLastErr = "Errore ricezione pacchetto video";
      m_st = Status::kERROR;
      break;
    }
  }

  // check status packets number change
  if (m_szNStatusPackets <= m_szNStatusPacketsLast) {
    if (m_timerKeepAlive.elapsed().count() 
          >= DILLOXL_TELLOCOMM_LINK_DOWN_TIMEOUT_S) {
      m_strLastErr = "Collegamento interrotto";
      m_bLinkAlive = false;
      m_st = Status::kERROR;
      m_timerKeepAlive = {}; // reset timer
    } else {
      m_bLinkAlive = true;
    }
  } else {
    m_timerKeepAlive = {}; // reset timer
  }

  // store current number of status packets and check timeout
  m_szNStatusPacketsLast = m_szNStatusPackets;

  // check anomaly on waiting for response
  if (m_bWaitForResponse && m_bControlPktSent) {
    if (m_timerW4R.elapsed().count()
          >= DILLOXL_TELLOCOMM_WAITFORRESP_TIMEOUT_S) {
      m_strLastErr = "Tempo massimo risposta superato";
      std::lock_guard<std::recursive_mutex> g{ m_mtxW4RCPS };
      m_bWaitForResponse = false;
      m_bControlPktSent  = false;
      m_timerW4R = {}; // reset timer
    }
  } else {
    m_timerW4R = {}; // reset timer
  }

#if 1
  // DEBUG-----
  static bool bPre_W4S, bPre_CPS;
  static uint32_t uPre_STW4 = 0;
  {
    std::lock_guard<std::recursive_mutex> g{ m_mtxW4RCPS };
    if (( bPre_CPS !=  m_bControlPktSent) ||
        ( bPre_W4S != m_bWaitForResponse) ||
        (uPre_STW4 != m_uSeqToWaitFor)) {
      std::fprintf(stderr
        , DILLOXL_TERM_FGDIMM 
          "[COM]: W4R=%3s CPS=%3s SEQ=%8u" 
          DILLOXL_TERM_RESETA "\n"
        , m_bWaitForResponse ? "YES" : "NO"
        , m_bControlPktSent  ? "YES" : "NO"
        , m_uSeqToWaitFor);
    }
    bPre_CPS  = m_bControlPktSent;
    bPre_W4S  = m_bWaitForResponse;
    uPre_STW4 = m_uSeqToWaitFor;
  }
#endif  
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusGetInfo()
{
  uint8_t btData[128];
  size_t szInBytes = 0;

  // ...wait for response to activation command
  auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
  auto ret = m_SockControl.receive(btData, sizeof(btData)
    , szInBytes, from, u2p);
  switch (ret) {
  case sf::Socket::Status::Done:
    btData[std::min(sizeof(btData) - 1, szInBytes)] = 0;
    std::snprintf(m_strSerial, sizeof(m_strSerial)
      , "%s", reinterpret_cast<const char*>(btData));
    m_strLastErr = "Nessuno";
    m_st = Status::kWORKING;
    m_szNControlPacketsIn++;
    m_bWaitForResponse = false;
    m_bControlPktSent  = false;
    m_uSeqToWaitFor    = 0;
#if DILLOXL_TELLOCOMM_DUMP_DEBUG == 1
    dump_data("CTRL", btData, szInBytes);
#endif    
    break;
  case sf::Socket::Status::NotReady:
    break;
  default:
    m_strLastErr = "Errore ricezione risposta a informazioni";
    m_st = Status::kERROR;
    break;
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusError()  
{
  m_st = Status::kUNDEFINED;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusUndefined()
{
  m_st = Status::kUNDEFINED;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_DequeueAndSend()
{
  std::lock_guard<std::recursive_mutex> g{ m_mtxQ };
  if (!m_qMsg.empty()) {
    auto msg = m_qMsg.front(); m_qMsg.pop();
    m_Send(msg);
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_Send(const std::string& msg)
{
  auto to = sf::IpAddress::resolve(kTELLO_IPv4);
  if (Status::kWORKING == m_st) {
    if (m_SockControl.send(msg.c_str(), msg.size() + 1 // notice the +1 here
      , to.value(), kUDP_PORT_CONTROL)  != sf::Socket::Status::Done) 
    {
      std::lock_guard<std::recursive_mutex> g{ m_mtxW4RCPS };
      m_strLastErr = "Invio messaggio utente fallito";
      m_st = Status::kERROR;
      m_bWaitForResponse = false;
      m_bControlPktSent  = false;
      m_uSeqToWaitFor    = 0;
      m_timerW4R         = {}; // reset timer
    } else {
      std::lock_guard<std::recursive_mutex> g{ m_mtxW4RCPS };
      m_strLastErr = "Attesa risposta...";
      m_bWaitForResponse = true;
      m_bControlPktSent  = true;
      m_timerW4R         = {}; // reset timer
      m_uSeqToWaitFor++;
      m_szNControlPacketsOt++;
#if DILLOXL_TELLOCOMM_QUEUE_DEBUG == 1
      std::fprintf(stderr
        , DILLOXL_TERM_FGDIMM
          "[COM]: Messaggio \"%s\" inviato (Seq=%u). Attesa risposta..." 
          DILLOXL_TERM_RESETA "\n"
        , msg.c_str(), m_uSeqToWaitFor);
#endif        
    }
  }
}
