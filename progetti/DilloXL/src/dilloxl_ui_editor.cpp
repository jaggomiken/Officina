/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_ui_editor.h"
#include "dilloxl_user_program.h"
#include "TextEditor.h"

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATIONS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::GuiEditor::Impl {
public:
  Impl();
  ~Impl();
  void draw();

  TextEditor m_editor;
  TextEditor::LanguageDefinition m_ld;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiEditor::GuiEditor()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiEditor::GuiEditor(GuiEditor&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiEditor& dilloxl::GuiEditor::operator=(GuiEditor&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiEditor::~GuiEditor()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiEditor::draw()
{
  m_pImpl->draw();
  if (m_pImpl->m_editor.IsTextChanged()) {
    UserProgram::GetCurrent().setSource(m_pImpl->m_editor.GetText());
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiEditor::setSource(const std::string& value)
{
  m_pImpl->m_editor.SetText(value);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::GuiEditor::source() const
{
  return m_pImpl->m_editor.GetText();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiEditor::Impl::Impl()
{
  m_ld = TextEditor::LanguageDefinition::CPlusPlus();
  m_editor.SetLanguageDefinition(m_ld);
	m_editor.SetPalette(TextEditor::GetLightPalette());
	m_editor.SetReadOnly(false); // insert-text non funziona se è read-only
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiEditor::Impl::~Impl()
{

}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiEditor::Impl::draw()
{
	auto cpos = m_editor.GetCursorPosition();
	ImGui::Begin("PROGRAMMA", nullptr
		, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Modifica")) {
			bool ro = m_editor.IsReadOnly();
			if (ImGui::MenuItem("Sola-Lettura", nullptr, &ro)) { m_editor.SetReadOnly(ro); }
			ImGui::Separator();
			if (ImGui::MenuItem("Annulla", "ALT-Backspace", nullptr, !ro && m_editor.CanUndo())) {
				m_editor.Undo();
			}
			if (ImGui::MenuItem("Rifai", "Ctrl-Y", nullptr, !ro && m_editor.CanRedo())) {
				m_editor.Redo();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Copia", "Ctrl-C", nullptr, m_editor.HasSelection())) {
				m_editor.Copy();
			}
			if (ImGui::MenuItem("Taglia", "Ctrl-X", nullptr, !ro && m_editor.HasSelection())) {
				m_editor.Cut();
			}
			if (ImGui::MenuItem("Cancella", "Del", nullptr, !ro && m_editor.HasSelection())) {
				m_editor.Delete();
			}
			if (ImGui::MenuItem("Incolla", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr)) {
				m_editor.Paste();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Seleziona Tutto", nullptr, nullptr)) {
				m_editor.SetSelection(TextEditor::Coordinates()
					, TextEditor::Coordinates(m_editor.GetTotalLines(), 0));
			}
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Vista")) {
			if (ImGui::MenuItem("Scura")) {
				m_editor.SetPalette(TextEditor::GetDarkPalette());
			}
			if (ImGui::MenuItem("Chiara")) {
				m_editor.SetPalette(TextEditor::GetLightPalette());
			}
			if (ImGui::MenuItem("Retro")) {
				m_editor.SetPalette(TextEditor::GetRetroBluePalette());
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGui::Text("%6d/%-6d %6d linee  | %s | %s | %s | %s"
		, cpos.mLine + 1, cpos.mColumn + 1, m_editor.GetTotalLines()
		, m_editor.IsOverwrite() ? "Ovr" : "Ins"
		, m_editor.CanUndo() ? "*" : " "
		, m_editor.GetLanguageDefinition().mName.c_str(), "PROGRAMMA.dillo");

	m_editor.Render("User Program");
	ImGui::End();
}
