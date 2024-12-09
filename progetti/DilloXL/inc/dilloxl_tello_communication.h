/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_tello_communication_h
#define dilloxl_tello_communication_h

#include "dilloxl_system.h"
#include <SFML/Network.hpp>

namespace dilloxl {
  struct TelloCommunication {
    static constexpr       size_t kSTATUS_STRING_SIZE    = 21;
    static constexpr     uint16_t kUDP_PORT_CONTROL      = 8889;
    static constexpr     uint16_t kUDP_PORT_STATUS       = 8890;
    static constexpr     uint16_t kUDP_PORT_VIDEO        = 11111;
    static constexpr  const char* kTELLO_IPv4            = "192.168.10.1";

    enum class SendMode : uint32_t { kNORMAL = 0, kFORCE = 1 };
    enum class Status : uint32_t {
        kUNDEFINED   = 0
      , kUNCONNECTED = 1
      , kCONNECTING  = 2
      , kCONNECTED   = 3
      , kGETINFO     = 4
      , kWORKING     = 5
      , kERROR       = 6
    };

    using ContrlCallback = std::function<void(const uint8_t*,size_t)>;
    using StatusCallback = std::function<void(const uint8_t*,size_t)>;
    using VideoSCallback = std::function<void(const uint8_t*,size_t)>;

    static std::string StatusToString(Status);

    TelloCommunication();
    TelloCommunication(const TelloCommunication&) = delete;
    TelloCommunication(TelloCommunication&&) = delete;
    TelloCommunication& operator=(const TelloCommunication&) = delete;
    TelloCommunication& operator=(TelloCommunication&&) = delete;
   ~TelloCommunication();

    void reset();
    void tryLink();
    void execute();
    const std::string& lastError() const;
    std::string lastStatus() const;
    std::string serial() const;

    size_t  nCtrlPktsIn() const;
    size_t nCtrlPktsOut() const;
    size_t  nStatusPkts() const;
    size_t   nVideoPkts() const;
                                                  // false -> timeout, true -> ready
    bool waitforready(size_t timeout_50ms = 200); // thread-safe (must be invoked by another thread)
    void send(const std::string&, SendMode = SendMode::kNORMAL); // thread-safe
    void setContrlCallback(const ContrlCallback&);
    void setStatusCallback(const StatusCallback&);
    void setVideosCallback(const VideoSCallback&);

    bool isLinkAlive() const;
    bool isWaitingForResponse() const;

  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_tello_communication_h
