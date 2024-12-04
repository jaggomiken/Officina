/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_if.h"
#include "dilloxl_user_program.h"
#include "dilloxl_system.h"

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_print_message(const char* msg) {
  std::fprintf(stdout, "[DILLOXL]: %s\n", msg);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_left(float value) {
  std::fprintf(stdout, "[DILLOXL]: SINISTRA %.3f cm\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_right(float value) {
  std::fprintf(stdout, "[DILLOXL]: DESTRA %.3f cm\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_up(float value) {
  std::fprintf(stdout, "[DILLOXL]: SU %.3f cm\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_move_down(float value) {
  std::fprintf(stdout, "[DILLOXL]: GIU' %.3f cm\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_rotate_cw(float value) {
  std::fprintf(stdout, "[DILLOXL]: SENSO-ORARIO %.3f gradi\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_rotate_cc(float value) {
  std::fprintf(stdout, "[DILLOXL]: SENSO-ANTI-ORARIO %.3f gradi\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_takeoff(float value) {
  std::fprintf(stdout, "[DILLOXL]: DECOLLA (%.3f)\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_land(float value) {
  std::fprintf(stdout, "[DILLOXL]: ATTERRA (%.3f)\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_forward(float value) {
  std::fprintf(stdout, "[DILLOXL]: VAI-AVANTI %.3f cm\n", value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static void dilloxl_drone_backward(float value) {
  std::fprintf(stdout, "[DILLOXL]: VAI-INDIETRO %.3f cm\n", value);
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
  gpUPC = pctx;
}
