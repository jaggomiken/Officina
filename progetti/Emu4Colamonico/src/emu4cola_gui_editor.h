/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef emu4cola_gui_editor_h
#define emu4cola_gui_editor_h

#include "emu4cola_incs.h"
#include "emu4cola_context.h"

namespace cola
{
  class GuiEditor {
  public:
    GuiEditor();
    GuiEditor(const GuiEditor&) = delete;
    GuiEditor(GuiEditor&&) noexcept;
    GuiEditor& operator=(const GuiEditor&) = delete;
    GuiEditor& operator=(GuiEditor&&) noexcept;
    virtual ~GuiEditor();
    virtual void draw();
  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // emu4cola_gui_editor_h
