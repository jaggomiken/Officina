/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_tello_communication.h"

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * MACROS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define DILLOXL_TELLECOMM_DEBUG                                            0

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
: m_st                  { Status::kUNDEFINED }
, m_szNControlPacketsIn {                  0 }
, m_szNControlPacketsOt {                  0 }
, m_szNStatusPackets    {                  0 }
, m_szNVideoPackets     {                  0 }
{

}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloCommunication::~TelloCommunication()
{
  
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::tryLink()
{
  m_SockControl.unbind();
   m_SockStatus.unbind();
    m_SockVideo.unbind();
  m_st = Status::kUNCONNECTED;
  if (m_SockControl.bind(kUDP_PORT_CONTROL) != sf::Socket::Status::Done) {
    m_strLastErr = "Problemi sul socket di controllo";
    m_st = Status::kERROR;
    return;
  }
  if (m_SockStatus.bind(kUDP_PORT_STATUS) != sf::Socket::Status::Done) {
    m_strLastErr = "Problemi sul socket di stato";
    m_st = Status::kERROR;
    return;
  }
  if (m_SockVideo.bind(kUDP_PORT_VIDEO) != sf::Socket::Status::Done) {
    m_strLastErr = "Problemi sul socket video";
    m_st = Status::kERROR;
    return;
  }
  m_SockControl.setBlocking(false);
   m_SockStatus.setBlocking(false);
    m_SockVideo.setBlocking(false);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::execute()
{
  switch (m_st) {
  case Status::kUNCONNECTED: m_onStatusUnconnected(); break;
  case  Status::kCONNECTING: m_onStatusConnecting();  break;
  case   Status::kCONNECTED: m_onStatusConnected();   break;
  case     Status::kWORKING: m_onStatusWorking();     break;
  case   Status::kEMERGENCY: m_onStatusEmergency();   break;
  case       Status::kERROR: m_onStatusError();       break;
  default:                   m_onStatusUndefined();   break;
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
const std::string& dilloxl::TelloCommunication::lastError() const
{
  return m_strLastErr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::TelloCommunication::lastStatus() const
{
  return StatusToString(m_st);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::send(const std::string& msg)
{
  auto to = sf::IpAddress::resolve(kTELLO_IPv4);
  if (Status::kWORKING == m_st) {
    if (m_SockControl.send(msg.c_str(), msg.size(), to.value(), kUDP_PORT_CONTROL)
          != sf::Socket::Status::Done) 
    {
      m_strLastErr = "Invio messaggio utente fallito";
      m_st = Status::kERROR;
    } else {
      std::fprintf(stdout, "[COM]: Messages \"%s\" sent.\n", msg.c_str());
    }
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::setContrlCallback(const ContrlCallback& cb)
{
  m_contrlcb = cb;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::setStatusCallback(const StatusCallback& cb)
{
  m_statuscb = cb;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::setVideosCallback(const VideoSCallback& cb)
{
  m_videoscb = cb;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::m_onStatusUnconnected()
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
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::m_onStatusConnecting()
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
#if DILLOXL_TELLECOMM_DEBUG == 1
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
void dilloxl::TelloCommunication::m_onStatusConnected() 
{
  m_st = Status::kWORKING;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::m_onStatusWorking()  
{
  static uint8_t btData[1024];
  size_t szInBytes = 0;

  // ...wait for control packets
  { auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
    auto ret = m_SockControl.receive(btData, sizeof(btData), szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done:
      m_strLastErr = "Nessuno";
      m_st = Status::kWORKING;
      m_szNControlPacketsIn++;
#if DILLOXL_TELLECOMM_DEBUG == 1
      dump_data("CONTROL", btData, szInBytes);
#endif
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

  // ...wait for status packets
  { auto from = sf::IpAddress::resolve({}); uint16_t u2p = 0;
    auto ret = m_SockStatus.receive(btData, sizeof(btData), szInBytes, from, u2p);
    switch (ret) {
    case sf::Socket::Status::Done:
      m_strLastErr = "Nessuno";
      m_st = Status::kWORKING;
      m_szNStatusPackets++;
#if DILLOXL_TELLECOMM_DEBUG == 1
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
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::m_onStatusEmergency()
{

}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::m_onStatusError()  
{

}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloCommunication::m_onStatusUndefined()
{

}
