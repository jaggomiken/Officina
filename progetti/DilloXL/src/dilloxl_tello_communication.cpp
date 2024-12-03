/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_tello_communication.h"
#include <queue>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * MACROS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define DILLOXL_TELLOCOMM_DEBUG                                            0
#define DILLOXL_TELLOCOMM_LINK_DOWN_TIMEOUT_S                              2
#define DILLOXL_TELLOCOMM_WAITFORRESP_TIMEOUT_S                            2

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
  {}

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
  Timer m_timerKeepAlive;
  bool m_bWaitForResponse;
  Timer m_timerW4R;
  std::queue<std::string> m_qMsg;

  void m_OnStatusUnconnected();
  void m_OnStatusConnecting(); 
  void m_OnStatusConnected();  
  void m_OnStatusWorking();    
  void m_OnStatusEmergency();  
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
  case Status::kEMERGENCY  : return "IN EMERGENZA";
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
void dilloxl::TelloCommunication::tryLink()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_strLastErr = "Nessuno";
  m_pImpl->m_szNControlPacketsIn  =     0;
  m_pImpl->m_szNControlPacketsOt  =     0;
  m_pImpl->m_szNStatusPackets     =     0;
  m_pImpl->m_szNVideoPackets      =     0;
  m_pImpl->m_szNStatusPacketsLast =     0;
  m_pImpl->m_bLinkAlive           = false;
  m_pImpl->m_bWaitForResponse     = false;
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
  case   Status::kEMERGENCY: m_pImpl->m_OnStatusEmergency();   break;
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
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::send(const std::string& msg
  , SendMode mode)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  if (SendMode::kFORCE == mode) {
    std::fprintf(stderr, "\n[COM]: MESSAGGIO URGENTE \"%s\"...\n", msg.c_str());
    m_pImpl->m_Send(msg);
  } else {
    if (m_pImpl->m_bWaitForResponse) {
      std::fprintf(stderr, "[COM]: Messaggio \"%s\" accodato.\n", msg.c_str());
      m_pImpl->m_qMsg.push(msg);
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
void dilloxl::TelloCommunication::Impl::m_OnStatusUnconnected()
{
  // send activation command...
  std::string msg{ "command" };
  auto to = sf::IpAddress::resolve(kTELLO_IPv4);
  if (m_SockControl.send(msg.c_str(), msg.size(), to.value(), kUDP_PORT_CONTROL)
        != sf::Socket::Status::Done) 
  {
    m_strLastErr = "Invio messaggio attivazione fallito";
    m_st = Status::kERROR;
  } else {
    m_strLastErr = "Nessuno";
    m_st = Status::kCONNECTING;
    m_szNControlPacketsOt++;
    m_bWaitForResponse = true;
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
  auto ret = m_SockControl.receive(btData, sizeof(btData), szInBytes, from, u2p);
  switch (ret) {
  case sf::Socket::Status::Done:
    m_strLastErr = "Nessuno";
    m_st = Status::kCONNECTED;
    m_szNControlPacketsIn++;
    m_bWaitForResponse = false;
#if DILLOXL_TELLOCOMM_DEBUG == 1
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
  m_st = Status::kWORKING;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusWorking()  
{
  static uint8_t btData[1024];
  size_t szInBytes = 0;

  // check and dequeue queued messages
  if (!m_bWaitForResponse) { m_DequeueAndSend(); }

  // ...wait for control packets
  { auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
    auto ret = m_SockControl.receive(btData, sizeof(btData), szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done:
  #if DILLOXL_TELLOCOMM_DEBUG == 1
        dump_data("CONTROL", btData, szInBytes);
  #endif
      if (m_bWaitForResponse) {
        m_strLastErr = "Nessuno";
        btData[std::min(sizeof(btData) - 1, szInBytes)] = 0;
        std::fprintf(stdout, "%s\n", btData);
        std::fflush (stdout);
      } else {
        m_strLastErr = "Pacchetto di controllo inatteso";
      }
      m_st = Status::kWORKING;
      m_szNControlPacketsIn++;
      m_bWaitForResponse = false;
      if (m_contrlcb) { m_contrlcb(btData, szInBytes); }
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
    auto ret = m_SockStatus.receive(btData, sizeof(btData), szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done:
      m_strLastErr = "Nessuno";
      m_st = Status::kWORKING;
      m_szNStatusPackets++;
#if DILLOXL_TELLOCOMM_DEBUG == 1
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
    auto ret = m_SockVideo.receive(btData, sizeof(btData), szInBytes, from, u2p);
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
  if (m_bWaitForResponse) {
    if (m_timerW4R.elapsed().count()
          >= DILLOXL_TELLOCOMM_WAITFORRESP_TIMEOUT_S) {
      m_strLastErr = "Tempo massimo risposta superato";
      m_bWaitForResponse = false;
      m_timerW4R = {}; // reset timer
    }
  } else {
    m_timerW4R = {}; // reset timer
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::Impl::m_OnStatusEmergency()
{

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
    if (m_SockControl.send(msg.c_str(), msg.size()
      , to.value(), kUDP_PORT_CONTROL)  != sf::Socket::Status::Done) 
    {
      m_strLastErr = "Invio messaggio utente fallito";
      m_st = Status::kERROR;
      m_bWaitForResponse = false;
    } else {
      std::fprintf(stdout
        , "[COM]: Messaggio \"%s\" inviato. Risposta: ", msg.c_str());
      std::fflush (stdout);
      m_strLastErr = "Attesa risposta...";
      m_bWaitForResponse = true;
      m_szNControlPacketsOt++;
    }
  }
}
