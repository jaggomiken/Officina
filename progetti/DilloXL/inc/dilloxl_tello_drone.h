/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_tello_drone_h
#define dilloxl_tello_drone_h

#include "dilloxl_system.h"

namespace dilloxl {
  struct TelloCommunication;
  struct TelloDrone {
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

    static TelloDrone& Get();

    explicit TelloDrone(TelloCommunication&);
    TelloDrone(const TelloDrone&) = delete;
    TelloDrone(TelloDrone&&) = delete;
    TelloDrone& operator=(const TelloDrone&) = delete;
    TelloDrone& operator=(TelloDrone&&) = delete;
   ~TelloDrone();

    // commands with priority (FORCE)
    void takeoff();
    void land();
    void emergency();
    void reset();

    // commands without priority (ENQUEUE)
    void stop();
    void move_left(float);
    void move_right(float);
    void move_up(float);
    void move_down(float);
    void rotate_cw(float);
    void rotate_cc(float);
    void forward(float);
    void backward(float);
    void video(bool);

    bool isActive() const;
    const Status& lastStatus() const;
    const std::string& lastCommandResult() const;
    TelloCommunication& com();

  private:
    class Impl; Impl* m_pImpl;
  };
}

void DroneWaitAndSend(const std::function<void()>&);

#endif // dilloxl_tello_drone_h
