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
    void reset();
    
  private:
    class Impl; Impl* m_pImpl;
  };
}

void DroneWaitAndSend(const std::function<void()>&);

#endif // dilloxl_tello_drone_h
