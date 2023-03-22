/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "dilloatello_gui_console.h"
#include "TextEditor.h"

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  *
  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
class dillo::GuiEditor::Impl {
public:
  Impl();
  ~Impl();
  void draw();

  TextEditor m_editor;
  TextEditor::LanguageDefinition m_ld;
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiEditor::GuiEditor()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLO_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiEditor::GuiEditor(GuiEditor&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiEditor& dillo::GuiEditor::operator=(GuiEditor&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiEditor::~GuiEditor()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void dillo::GuiEditor::draw()
{
  m_pImpl->draw();
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiEditor::Impl::Impl()
{
  m_ld = TextEditor::LanguageDefinition::CPlusPlus();
  m_editor.SetLanguageDefinition(m_ld);
	m_editor.SetPalette(TextEditor::GetLightPalette());
	m_editor.SetReadOnly(false); // insert-text non funziona se è read-only
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiEditor::Impl::~Impl()
{

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void dillo::GuiEditor::Impl::draw()
{
	auto cpos = m_editor.GetCursorPosition();
	ImGui::Begin("DILLO-A-TELLO", nullptr
		, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
	ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Modifica"))
		{
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

		if (ImGui::BeginMenu("Vista"))
		{
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

	auto msg = scoda_prossimo_messaggio();
	if (!msg.empty()) {
		m_editor.MoveBottom();
		m_editor.InsertText(msg);
	}

	m_editor.Render("TextEditor");
	ImGui::End();
}
