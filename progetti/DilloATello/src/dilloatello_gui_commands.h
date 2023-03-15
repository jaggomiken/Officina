/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 

#ifndef dilloatello_gui_commands_h
#define dilloatello_gui_commands_h

#include "dilloatello_sistema.h"

namespace dillo
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

#endif // dilloatello_gui_commands_h
