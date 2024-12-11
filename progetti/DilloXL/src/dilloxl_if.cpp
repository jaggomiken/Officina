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
#include "dilloxl_if.h"
#include "dilloxl_user_program.h"
#include "dilloxl_system.h"
#include "dilloxl_tello_drone.h"

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * MACROS (WAITFOR_MS must be 100)
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define DILLOXL_IF_DEBUG                                                   0
#define DILLOXL_IF_WAITFOR_MS                                            100

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * NOTICE: These functions are invoked by another thread different from the
 * main thread that runs the GUI and communication code. The other thread is
 * specialized in running the user program. The DILLOXL_IF_WAITFOR_MS is
 * used for fine tuning, but the most important thing is that the COM is
 * able to wait for a response from the drone. So DILLOXL_IF_WAITFOR_MS is 0
 * for that reason. The best value for now is 100ms.
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_print_message(const char* msg) {
  std::fprintf(stderr
    , DILLOXL_TERM_GRNBLK "[PRO]: %s" DILLOXL_TERM_RESETA "\n", msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_stop(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().stop();
#if DILLOXL_IF_DEBUG == 1  
    std::fprintf(stderr, "[PRO]: STOP (%.3f): INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_left(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().move_left(value);
#if DILLOXL_IF_DEBUG == 1    
    std::fprintf(stderr, "[PRO]: SINISTRA %.3f cm: INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_right(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().move_right(value);
#if DILLOXL_IF_DEBUG == 1    
    std::fprintf(stderr, "[PRO]: DESTRA %.3f cm: INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_up(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().move_up(value);
#if DILLOXL_IF_DEBUG == 1
    std::fprintf(stderr, "[PRO]: SU %.3f cm: INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_down(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().move_down(value);
#if DILLOXL_IF_DEBUG == 1      
    std::fprintf(stderr, "[PRO]: GIU' %.3f cm: INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_rotate_cw(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().rotate_cw(value);
#if DILLOXL_IF_DEBUG == 1      
    std::fprintf(stderr
      , "[PRO]: SENSO-ORARIO %.3f gradi: INSERITO\n"
      , value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_rotate_cc(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().rotate_cc(value);
#if DILLOXL_IF_DEBUG == 1      
    std::fprintf(stderr
      , "[PRO]: SENSO-ANTI-ORARIO %.3f gradi: INSERITO\n"
      , value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_takeoff(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().takeoff();
#if DILLOXL_IF_DEBUG == 1  
    std::fprintf(stderr, "[PRO]: DECOLLA (%.3f): INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_land(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().land();
#if DILLOXL_IF_DEBUG == 1      
    std::fprintf(stderr, "[PRO]: ATTERRA (%.3f): INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_forward(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().forward(value);
#if DILLOXL_IF_DEBUG == 1  
    std::fprintf(stderr, "[PRO]: VAI-AVANTI %.3f cm: INSERITO\n", value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_backward(float value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().backward(value);
#if DILLOXL_IF_DEBUG == 1      
    std::fprintf(stderr, "[PRO]: VAI-INDIETRO %.3f cm: INSERITO\n"
      , value);
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_video(bool value) {
  DroneWaitAndSend([=]() {
    std::this_thread::sleep_for(
      std::chrono::milliseconds(DILLOXL_IF_WAITFOR_MS));
    dilloxl::TelloDrone::Get().video(value);
#if DILLOXL_IF_DEBUG == 1      
    std::fprintf(stderr, "[PRO]: VIDEO %s: INSERITO\n"
      , value ? "ABILITATO" : "DISABILITATO");
#endif  
  });
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static UserProgramContext* gpUPC = nullptr;
UserProgramContext* UserProgramContext::Get()
{
  DILLOXL_CAPTURE_CPU(nullptr == gpUPC, "Puntatore a UPC NULL");
  return gpUPC;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::SetupUserProgramContext(UserProgramContext* pctx, void* puser)
{
  pctx->pOpaque = puser;
  pctx->print_message    = &dilloxl_print_message;
  pctx->drone_takeoff    = &dilloxl_drone_takeoff;
  pctx->drone_land       = &dilloxl_drone_land;
  pctx->drone_rotate_cw  = &dilloxl_drone_rotate_cw;
  pctx->drone_rotate_cc  = &dilloxl_drone_rotate_cc;
  pctx->drone_move_up    = &dilloxl_drone_move_up;
  pctx->drone_move_down  = &dilloxl_drone_move_down;
  pctx->drone_move_left  = &dilloxl_drone_move_left;
  pctx->drone_move_right = &dilloxl_drone_move_right;
  pctx->drone_forward    = &dilloxl_drone_forward;
  pctx->drone_backward   = &dilloxl_drone_backward;
  pctx->drone_stop       = &dilloxl_drone_stop;
  pctx->drone_video      = &dilloxl_drone_video;
  gpUPC = pctx;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * CLASS IMPLEMENTATION (to allow crea_oggetto_da_astrazione())
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
Drone::Drone()
: m_pctx{ nullptr }
{
  m_pctx = UserProgramContext::Get();
  DILLOXL_CAPTURE_CPU(nullptr == m_pctx
    , "Contesto di programma utente non disponibile");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHODS IMPLEMENTATION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void Drone::           vai_a_sinistra_cm(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_move_left(v); }
void Drone::             vai_a_destra_cm(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_move_right(v); }
void Drone::                   vai_su_cm(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_move_up(v); }
void Drone::                  vai_giu_cm(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_move_down(v); }
void Drone::    ruota_senso_orario_gradi(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_rotate_cw(v); }
void Drone::ruota_senso_antiorario_gradi(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_rotate_cc(v); }
void Drone::               vai_avanti_cm(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_forward(v); }
void Drone::             vai_indietro_cm(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_backward(v); }
void Drone::                     decolla(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_takeoff(v); }
void Drone::                     atterra(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_land(v); }
void Drone::                        stop(float v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_stop(v); }
void Drone::                       video( bool v) { DILLOXL_CAPTURE_CPU(nullptr == m_pctx, "NULL"); m_pctx->drone_video(v); }
