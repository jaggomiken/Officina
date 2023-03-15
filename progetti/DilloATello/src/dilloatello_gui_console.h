/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef dilloatello_gui_console_h
#define dilloatello_gui_console_h

#include "dilloatello_sistema.h"

namespace dillo
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

#endif // dillo::
