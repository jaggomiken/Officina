/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef emu4cola_gui_memory_h
#define emu4cola_gui_memory_h

#include "emu4cola_incs.h"
#include "emu4cola_context.h"

namespace cola
{
  class GuiMemoryView {
  public:
    GuiMemoryView();
    GuiMemoryView(const GuiMemoryView&) = delete;
    GuiMemoryView(GuiMemoryView&&) noexcept;
    GuiMemoryView& operator=(const GuiMemoryView&) = delete;
    GuiMemoryView& operator=(GuiMemoryView&&) noexcept;
    virtual ~GuiMemoryView();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // emu4cola_gui_memory_h