/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef emu4cola_gui_ports_h
#define emu4cola_gui_ports_h

#include "emu4cola_incs.h"
#include "emu4cola_context.h"

namespace cola
{
  class GuiPortsAndIrqView {
  public:
    GuiPortsAndIrqView();
    GuiPortsAndIrqView(const GuiPortsAndIrqView&) = delete;
    GuiPortsAndIrqView(GuiPortsAndIrqView&&) noexcept;
    GuiPortsAndIrqView& operator=(const GuiPortsAndIrqView&) = delete;
    GuiPortsAndIrqView& operator=(GuiPortsAndIrqView&&) noexcept;
    virtual ~GuiPortsAndIrqView();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // emu4cola_gui_ports_h