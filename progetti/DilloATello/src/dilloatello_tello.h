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
  static TelloDrone* GetTello();

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
  
  void wait_for_result(size_t ms = 100, size_t tmout = 10000);
  const Status& lastStatus() const {
    std::lock_guard<std::mutex> guard{ m_MtxStatus};
    return m_Status; 
  }

private:
  static std::mutex m_mtxDrone;
  static TelloDrone* m_pDrone;
  static void SetDronePointer(TelloDrone*);

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

int programma_tello();

#endif // tello_h
