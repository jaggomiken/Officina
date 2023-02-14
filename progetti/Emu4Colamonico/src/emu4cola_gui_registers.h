/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef emu4cola_gui_registers_h
#define emu4cola_gui_registers_h

#include "emu4cola_incs.h"
#include "emu4cola_context.h"

namespace cola
{
  class GuiRegistersView {
  public:
    GuiRegistersView();
    GuiRegistersView(const GuiRegistersView&) = delete;
    GuiRegistersView(GuiRegistersView&&) noexcept;
    GuiRegistersView& operator=(const GuiRegistersView&) = delete;
    GuiRegistersView& operator=(GuiRegistersView&&) noexcept;
    virtual ~GuiRegistersView();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // emu4cola_gui_registers_h