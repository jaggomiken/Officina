/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_ui_console_h
#define dilloxl_ui_console_h

#include "dilloxl_system.h"

namespace dilloxl {
  class GuiConsole {
  public:
    GuiConsole();
    GuiConsole(const GuiConsole&) = delete;
    GuiConsole(GuiConsole&&) noexcept;
    GuiConsole& operator=(const GuiConsole&) = delete;
    GuiConsole& operator=(GuiConsole&&) noexcept;
    virtual ~GuiConsole();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_ui_console_h
