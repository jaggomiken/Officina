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
: m_com{ com }
{
  gpDrone = this;
  ::memset(&m_status, 0, sizeof(m_status));
  com.setStatusCallback([=](const uint8_t* pData, size_t szSizeInByte) {
    std::string str{ reinterpret_cast<const char*>(pData), szSizeInByte };
    if (ParseStatus(str, m_status)) {
      m_bIsActive = true;
      m_status.sequence++;
    } else {
      m_bIsActive = false;
    }
  });
  com.setContrlCallback([=](const uint8_t* pData, size_t szSizeInByte) {
    m_strLastCmdRes = std::string{ 
      reinterpret_cast<const char*>(pData), szSizeInByte };
  });  
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::TelloDrone::~TelloDrone()
{

}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::takeoff()
{
  m_com.send("takeoff");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::land()
{
  m_com.send("land");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::emergency()
{
  m_com.send("emergency");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::TelloDrone::reset()
{
  ::memset(&m_status, 0, sizeof(m_status));
  m_strLastCmdRes = "";
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::TelloDrone::isActive() const
{
  return m_bIsActive;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
const dilloxl::TelloDrone::Status& dilloxl::TelloDrone::lastStatus() const
{
  return m_status;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
const std::string& dilloxl::TelloDrone::lastCommandResult() const
{
  return m_strLastCmdRes;
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
