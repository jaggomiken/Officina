/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_ui_control_h
#define dilloxl_ui_control_h

#include "dilloxl_system.h"

namespace dilloxl {
  class GuiControl {
  public:
    GuiControl();
    GuiControl(const GuiControl&) = delete;
    GuiControl(GuiControl&&) noexcept;
    GuiControl& operator=(const GuiControl&) = delete;
    GuiControl& operator=(GuiControl&&) noexcept;
    virtual ~GuiControl();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_ui_control_h
