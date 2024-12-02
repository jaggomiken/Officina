/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_ui_editor_h
#define dilloxl_ui_editor_h

#include "dilloxl_system.h"

namespace dilloxl {
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

#endif // dilloxl_ui_editor_h
