/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */

#ifndef tello_h
#define tello_h

#include <cstdint>
#include <cstddef>
#include <cstdio>
#include <functional>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <string>
#include <queue>
#include "UDPsocket.h"
#include "dilloatello_sistema.h"

std::vector<std::string> 
  split(std::string s, std::string delimiter) {
    size_t 
        pos_start = 0
      , pos_end
      , delim_len = delimiter.length();
    
    std::string token;
    std::vector<std::string> ret;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        ret.push_back(token);
    }

    ret.push_back(s.substr(pos_start));
    return ret;
}

class TelloDrone {
public:
  struct Status {
    float sequence; // aggiornato per ogni stato ricevuto
    union StatusAsArray {
      float values[21];
      struct StatusAsValues {
        float mid;
        float x;
        float y;
        float z;
        float unused; // mpry gestito in modo diverso perché Vec3
        float pitch;
        float roll;
        float yaw;
        float speed_x;
        float speed_y;
        float speed_z;
        float temperature_lo;
        float temperature_hi;
        float time_of_flight;
        float height_from_floor;
        float battery_level;
        float barometer_cm;
        float motor_time;
        float acceleration_x;
        float acceleration_y;
        float acceleration_z;
      } status;
    } u;
  };

  static constexpr size_t kSTATUS_SIZE = 21;
  static constexpr uint16_t kTELLO_CONTROL_PORT =  8889;
  static constexpr uint16_t kLOCAL_STATUS_PORT  =  8890;
  static constexpr uint16_t kLOCAL_VIDEOS_PORT  = 11111;
  static constexpr const char* kTELLO_IPv4 = "192.168.10.1";
  static constexpr const size_t kTELLO_SLEEP_BTW_CMDS_MS = 100;

  static void PrintStatus(const Status&);

  TelloDrone();
 ~TelloDrone();
  TelloDrone(const TelloDrone&)            = delete;
  TelloDrone(TelloDrone&&)                 = delete;
  TelloDrone& operator=(const TelloDrone&) = delete;
  TelloDrone& operator=(TelloDrone&&)      = delete;
  
  bool connect();
  bool takeoff();
  bool land();
  bool emergency();
  bool stop();
  bool go_L(float);
  bool go_R(float);
  bool go_U(float);
  bool go_D(float);
  bool rot_CW(float);
  bool rot_CC(float);
  bool forward(float);
  bool backward(float);
  bool flip(const std::string&);
  bool setSpeed(float);
  bool video_enable(bool);
  bool mission_pad_enable(bool);
  
  void wait_for_result(size_t ms = 100);
  const Status& lastStatus() const {
    std::lock_guard<std::mutex> guard{ m_MtxStatus};
    return m_Status; 
  }

private:
  Status m_Status;
  UDPsocket m_SocketControl;
  UDPsocket m_SocketStatus;
  UDPsocket m_SocketVideos;
  bool m_bControlMsgSent;
  std::thread m_ThreadControlResult;
  std::thread m_ThreadStatus;
  std::thread m_ThreadVideoStream;
  uint32_t m_uExitThreads;
  mutable std::mutex m_MtxStatus;
  mutable std::mutex m_MtxControl;
  std::queue<std::function<bool()>> m_qControl;
  std::function<bool()> m_fnCurCmd;
  bool m_bWaitForResponse;

  void m_SetupControlThread();
  void m_SetupStatusThread();
  void m_SetupVideoStreamThread();
  void m_ParseStatus(const std::string&);
};

void TelloDrone::PrintStatus(const Status& st)
{
  stampa_stato(
      "SEQ=%.0f MID(%.0f,%.0f,%.0f,%.0f) "
      "PRY(%.0f,%.0f,%.0f) "
      "VEL(%.0f,%.0f,%.0f) "
      "TEM(%.0f,%.0f) TOF=%.0f H=%.0f BTRY=%.0f BARO=%.0f MOTT=%.0f ACC(%.0f,%.0f,%.0f)\n"
    , st.sequence
    , st.u.status.mid
    , st.u.status.x
    , st.u.status.y
    , st.u.status.z
    , st.u.status.pitch
    , st.u.status.roll
    , st.u.status.yaw
    , st.u.status.speed_x
    , st.u.status.speed_y
    , st.u.status.speed_z
    , st.u.status.temperature_lo
    , st.u.status.temperature_hi
    , st.u.status.time_of_flight
    , st.u.status.height_from_floor
    , st.u.status.battery_level
    , st.u.status.barometer_cm
    , st.u.status.motor_time
    , st.u.status.acceleration_x
    , st.u.status.acceleration_y
    , st.u.status.acceleration_z
  );
}

TelloDrone::TelloDrone()
: m_bControlMsgSent { false }
, m_uExitThreads    {     0 }
, m_bWaitForResponse{ false }
{
  std::memset(&m_Status,0,sizeof(m_Status));
}

TelloDrone::~TelloDrone() {
  emergency();
}

bool TelloDrone::connect() {
  constexpr int kE = int(UDPsocket::Status::SocketError);

  // FASE 1
  stampa("CONTROLLO: Apertura socket...\n");
  if (kE == m_SocketControl.open()) {
    stampa_errore( "ERRORE SOCKET: open controllo !\n");
    return false;
  }
  if (kE == m_SocketStatus.open()) {
    stampa_errore( "ERRORE SOCKET: open status !\n");
    return false;
  }
  if (kE == m_SocketVideos.open()) {
    stampa_errore( "ERRORE SOCKET: open video-stream !\n");
    return false;
  }
  stampa("CONTROLLO: Apertura socket: OK.\n");

  // FASE 2
  stampa("CONTROLLO: Configurazione socket...\n");
  if (kE == m_SocketControl.bind(kTELLO_CONTROL_PORT)) {
    stampa_errore( "ERRORE SOCKET: bind controllo !\n");
    return false;
  }
  if (kE == m_SocketStatus.bind(kLOCAL_STATUS_PORT)) {
    stampa_errore( "ERRORE SOCKET: bind status !\n");
    return false;
  }
  if (kE == m_SocketVideos.bind(kLOCAL_VIDEOS_PORT)) {
    stampa_errore( "ERRORE SOCKET: bind video-stream !\n");
    return false;
  }
  stampa("CONTROLLO: Configurazione socket: OK.\n");

  // FASE 3, configurazione dei thread di governo
  m_SetupControlThread();
  m_SetupStatusThread();
  m_SetupVideoStreamThread();
  
  // FASE 4, attivazione TELLO
  stampa("CONTROLLO: Comunicazione con il Tello...\n");
  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("command")
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });

  // FINE
  return true;
}

bool TelloDrone::takeoff() {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("takeoff")
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::land() {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("land")
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::emergency() {
  wait_for_result(10);

  if (int(UDPsocket::Status::SendError) 
    == m_SocketControl.send(std::string("emergency")
    , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
    stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
    return false;
  } else {
    m_bControlMsgSent = true;
    stampa("CONTROLLO:  Attesa esito...\n");
  }
  return true;
}

bool TelloDrone::stop() {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("stop")
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::forward(float cm) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    std::string cmd = std::string("forward ") + std::to_string(int(cm));
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(cmd
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito per \"%s\"...\n", cmd.c_str());
      return true;
    }
  });
  return true;
}

bool TelloDrone::backward(float cm) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    std::string cmd = std::string("back ") + std::to_string(int(cm));
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(cmd
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::go_L(float cm) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    std::string cmd = std::string("left ") + std::to_string(int(cm));
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(cmd
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito per \"%s\"...\n", cmd.c_str());
      return true;
    }
  });
  return true;
}

bool TelloDrone::go_R(float cm) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    std::string cmd = std::string("right ") + std::to_string(int(cm));
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(cmd
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito per \"%s\"...\n",cmd.c_str());
      return true;
    }
  });
  return true;
}

bool TelloDrone::go_U(float cm) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    std::string cmd = std::string("up ") + std::to_string(int(cm));
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(cmd
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito per \"%s\"...\n", cmd.c_str());
      return true;
    }
  });
  return true;
}

bool TelloDrone::go_D(float cm) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    std::string cmd = std::string("down ") + std::to_string(int(cm));
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(cmd
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito per \"%s\"...\n", cmd.c_str());
      return true;
    }
  });
  return true;
}

bool TelloDrone::rot_CW(float deg) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("cw ") + std::to_string(deg)
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::rot_CC(float deg) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("ccw ") + std::to_string(deg)
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::flip(const std::string& w) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("flip ") + w
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::setSpeed(float v) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string("speed ") + std::to_string(v)
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::video_enable(bool enabled) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string(enabled ? "streamon" : "streamoff")
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

bool TelloDrone::mission_pad_enable(bool enabled) {
  wait_for_result(10);

  std::lock_guard<std::mutex> guard{ m_MtxControl };
  m_qControl.push([=]() {
    if (int(UDPsocket::Status::SendError) 
      == m_SocketControl.send(std::string(enabled ? "mon" : "moff")
      , UDPsocket::IPv4(kTELLO_IPv4,kTELLO_CONTROL_PORT))) {
      stampa_errore( "ERRORE COMUNICAZIONE: invio fallito !\n");
      return false;
    } else {
      m_bControlMsgSent = true;
      stampa("CONTROLLO:  Attesa esito...\n");
      return true;
    }
  });
  return true;
}

void TelloDrone::wait_for_result(size_t ms) {
  uint32_t uExitLoop = 0;
  while (0 == uExitLoop) {
    { std::lock_guard<std::mutex> guard{ m_MtxControl };
      if (!m_bWaitForResponse && !m_bControlMsgSent && !m_fnCurCmd) {
        uExitLoop = 1;
      } }
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
  }
}

void TelloDrone::m_SetupControlThread() {
  m_ThreadControlResult = std::thread([=]() {
    stampa("THREAD CONTROLLO ATTIVO.\n");
    while (0 == m_uExitThreads) {
      // variabili di stato locali
      uint32_t uGo = 0;

      // estrae il prossimo comando dalla coda...
      if (!m_bWaitForResponse) {
        std::lock_guard<std::mutex> guard{ m_MtxControl };
        if (!m_qControl.empty()) {
          m_fnCurCmd = m_qControl.front();
          m_qControl.pop();
          uGo = 1;
        }
        else {
          uGo = 0;
        }
      }

      // elabora l'elemento estratto dalla coda...
      if (1 == uGo) { 
        m_fnCurCmd(); 
        m_bWaitForResponse = true; 
      }

      // attende la risposta...
      if (m_bControlMsgSent && m_bWaitForResponse) {
        UDPsocket::IPv4 addr;
        std::string message;
        if (int(UDPsocket::Status::RecvError)
          == m_SocketControl.recv(message, addr)) {
          stampa_errore( "ERRORE SOCKET: ricezione controllo !\n");
        }
        else {
          stampa("TELLO: \"%s\"\n", message.c_str());
          std::lock_guard<std::mutex> guard{ m_MtxControl };
          m_bWaitForResponse = false;
          m_bControlMsgSent  = false;
          m_fnCurCmd = {};
        }
      }
    }
  });
}

void TelloDrone::m_SetupStatusThread() {
  m_ThreadStatus = std::thread([=]() {
    stampa("THREAD STATUS ATTIVO.\n");
    while (0 == m_uExitThreads) {
      UDPsocket::IPv4 addr;
      std::string message;
      if (int(UDPsocket::Status::RecvError) 
        == m_SocketStatus.recv(message,addr)) {
        stampa_errore( "ERRORE SOCKET: ricezione status !\n");
      }
      else {
        // rimuove il CRLF finale
        m_ParseStatus(message.substr(0,message.size() - 2));
        PrintStatus(lastStatus());
      }
    }
  });
}

void TelloDrone::m_SetupVideoStreamThread() {
  m_ThreadVideoStream = std::thread([=]() {
    stampa("THREAD VIDEO ATTIVO.\n");
    while (0 == m_uExitThreads) {
      UDPsocket::IPv4 addr;
      std::string message;
      if (int(UDPsocket::Status::RecvError) 
        == m_SocketVideos.recv(message,addr)) {
        stampa_errore( "ERRORE SOCKET: ricezione video !\n");
      }
      else {
        fwrite(message.c_str(),1,message.length(),stdout);
        fflush(stdout);
      }
    }
  });
}

void TelloDrone::m_ParseStatus(const std::string& in) {
  Status st;
  std::vector<std::string> vTokens = split(in, ";");
  
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
      auto vpair = split(token, ":");
      if (vpair.size() >= 2) {
        if (4 != szIndex) {
          st.u.values[szIndex] = std::stof(vpair.at(1));
        }
      } else {
        stampa_errore( ">>>>> STRANO: TOKEN=\"%s\"\n", token.c_str());
      }
      szIndex += 1;
    }
  }

  {
    std::lock_guard<std::mutex> guard{ m_MtxStatus};
    st.sequence = m_Status.sequence + 1;
    m_Status = st;
  }
}

#endif // tello_h
