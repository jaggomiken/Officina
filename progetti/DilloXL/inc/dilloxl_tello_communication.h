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
    static constexpr const size_t kTELLO_INTERCOMMAND_MS = 100;

    enum class Status : uint32_t {
        kUNDEFINED   = 0
      , kUNCONNECTED = 1
      , kCONNECTING  = 2
      , kCONNECTED   = 3
      , kWORKING     = 4
      , kEMERGENCY   = 5
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

    void tryLink();
    void execute();
    const std::string& lastError() const;
    std::string lastStatus() const;

    size_t  nCtrlPktsIn() const { return m_szNControlPacketsIn; }
    size_t nCtrlPktsOut() const { return m_szNControlPacketsOt; }
    size_t  nStatusPkts() const { return m_szNStatusPackets; }
    size_t   nVideoPkts() const { return m_szNVideoPackets; }

    void send(const std::string&);
    void setContrlCallback(const ContrlCallback&);
    void setStatusCallback(const StatusCallback&);
    void setVideosCallback(const VideoSCallback&);

  private:    
    sf::UdpSocket m_SockControl;
    sf::UdpSocket m_SockStatus;
    sf::UdpSocket m_SockVideo;
    Status m_st;
    std::string m_strLastErr;
    size_t m_szNControlPacketsIn;
    size_t m_szNControlPacketsOt;
    size_t m_szNStatusPackets;
    size_t m_szNVideoPackets;

    ContrlCallback m_contrlcb;
    StatusCallback m_statuscb;
    VideoSCallback m_videoscb;

    void m_onStatusUnconnected();
    void m_onStatusConnecting(); 
    void m_onStatusConnected();  
    void m_onStatusWorking();    
    void m_onStatusEmergency();  
    void m_onStatusError();      
    void m_onStatusUndefined();
  };
}

#endif // dilloxl_tello_communication_h
