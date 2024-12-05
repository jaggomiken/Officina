/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_tello_communication.h"
#include "dilloxl_tello_drone.h"
#include <memory.h>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATIONS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static bool ParseStatus(const std::string& in
  , dilloxl::TelloDrone::Status& st);

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::TelloDrone::Impl {
public:
  Impl(TelloCommunication& com)
  : m_com       {   com }
  , m_bIsActive { false }
  {
    ::memset(&m_status, 0, sizeof(m_status));
  }

 ~Impl() {
    // nothing to do for now
  }

  TelloCommunication& m_com;
  Status m_status;
  bool m_bIsActive;
  std::string m_strLastCmdRes;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloDrone* gpDrone = nullptr;
dilloxl::TelloDrone& dilloxl::TelloDrone::Get() {
  DILLOXL_CAPTURE_CPU(nullptr == gpDrone, "Nessun drone allocato");
  return *gpDrone;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloDrone::TelloDrone(TelloCommunication& com)
: m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{ com };
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Allocazione fallita");
  gpDrone = this;

  com.setStatusCallback([=](const uint8_t* pData, size_t szSizeInByte) {
    std::string str{ reinterpret_cast<const char*>(pData), szSizeInByte };
    if (ParseStatus(str, m_pImpl->m_status)) {
      m_pImpl->m_bIsActive = true;
      m_pImpl->m_status.sequence++;
    } else {
      m_pImpl->m_bIsActive = false;
    }
  });
  com.setContrlCallback([=](const uint8_t* pData, size_t szSizeInByte) {
    m_pImpl->m_strLastCmdRes = std::string{ 
      reinterpret_cast<const char*>(pData), szSizeInByte };
  });  
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloDrone::~TelloDrone()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::takeoff()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send("takeoff");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::land()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send("land", TelloCommunication::SendMode::kFORCE);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::emergency()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send("emergency", TelloCommunication::SendMode::kFORCE);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::reset()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.reset();
  ::memset(&m_pImpl->m_status, 0, sizeof(m_pImpl->m_status));
  m_pImpl->m_strLastCmdRes = "";
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::stop()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send("stop");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::move_left(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "left %.0f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::move_right(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "right %.0f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::move_up(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "up %.0f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::move_down(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "down %.0f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::rotate_cw(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "cw %.3f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::rotate_cc(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "ccw %.3f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::forward(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "forward %.0f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::backward(float value)
{
  char msg[64]; std::snprintf(msg, sizeof(msg), "back %.0f", value);
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_com.send(msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::TelloDrone::isActive() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_bIsActive && m_pImpl->m_com.isLinkAlive();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
const dilloxl::TelloDrone::Status& dilloxl::TelloDrone::lastStatus() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_status;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
const std::string& dilloxl::TelloDrone::lastCommandResult() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_strLastCmdRes;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloCommunication& dilloxl::TelloDrone::com()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_com;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static bool ParseStatus(const std::string& input
  , dilloxl::TelloDrone::Status& st)
{
  auto in = dilloxl::trim(input);
  std::vector<std::string> vTokens = dilloxl::split(in, ";");

  /*
   * Come restituito da TELLO, ecco le proprietà, nell'ordine di arrivo:
   *  0 mid:-1
   *  1 x:-100
   *  2 y:-100
   *  3 z:-100
   *  4 mpry:0,0,0
   *  5 pitch:0
   *  6 roll:0
   *  7 yaw:4
   *  8 vgx:0
   *  9 vgy:0
   * 10 vgz:0
   * 11 templ:41
   * 12 temph:45
   * 13 tof:10
   * 14 h:0
   * 15 bat:9
   * 16 baro:170.87
   * 17 time:0
   * 18 agx:7.00
   * 19 agy:12.00
   * 20 agz:-997.00
   */

  size_t szIndex = 0;
  for (const auto& token : vTokens) {
    if (!token.empty()) {
      auto vpair = dilloxl::split(token, ":");
      if (vpair.size() >= 2) {
        if (4 != szIndex) {
          st.u.values[szIndex] = std::stof(vpair.at(1));
        }
      }
      else {
        std::fprintf(stderr, ">>>>> STRANO: TOKEN=\"%s\"\n", token.c_str());
        return false;
      }
      szIndex += 1;
    }
  }
  return true;
}
