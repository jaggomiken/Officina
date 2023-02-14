/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 

#ifndef emu4cola_gui_commands_h
#define emu4cola_gui_commands_h

#include "emu4cola_incs.h"
#include "emu4cola_context.h"

namespace cola
{
  class GuiCommands {
  public:
    GuiCommands();
    GuiCommands(const GuiCommands&) = delete;
    GuiCommands(GuiCommands&&) noexcept;
    GuiCommands& operator=(const GuiCommands&) = delete;
    GuiCommands& operator=(GuiCommands&&) noexcept;
    virtual ~GuiCommands();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // emu4cola_gui_commands_h
