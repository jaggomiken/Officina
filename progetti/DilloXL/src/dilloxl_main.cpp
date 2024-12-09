/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_tello_communication.h"
#include "dilloxl_tello_drone.h"
#include "dilloxl_ui_console.h"
#include "dilloxl_ui_control.h"
#include "dilloxl_ui_editor.h"
#include "dilloxl_user_program.h"
#include <imgui.h>
#include <imgui-SFML.h>

int main(int argc, char* argv[]) 
{
  const char* pTitle = 
    "DILLO XL Versione 1.0.2 - "
    "By Prof. Michele Iacobellis (COLAMONICO-CHIARULLI)";
  std::fprintf(stderr, "[DILLOXL]: %s\n", pTitle);
  std::fprintf(stderr, "[DILLOXL]: argv[1] = valore scala display.\n");
  std::fprintf(stderr, "[DILLOXL]: argv[2] = limite framerate.\n");
  std::fprintf(stderr, "[DILLOXL]: E' preferibile mettere 0 perche' il "
    "traffico di rete e' gestito nel ciclo della GUI.\n");

  const float scale = std::atof(nullptr == argv[1] ? "1.5" : argv[1]);
  std::fprintf(stderr, "[DILLOXL]:  Valore di scala: %f\n", scale);
  const uint32_t uFRL = std::atoi(nullptr == argv[2] ? "0" : argv[2]);
  std::fprintf(stderr, "[DILLOXL]: Limite framerate: %u\n", uFRL);

  sf::RenderWindow window(sf::VideoMode({ 1800, 1000 }), pTitle);
  window.setFramerateLimit(uFRL);
  if (!ImGui::SFML::Init(window)) {
    std::fprintf(stderr, "[DILLOXL]: Errore di init.\n");
    return 1;
  }

  auto& style = ImGui::GetStyle();
  style.FrameRounding   = 3;
  style.ChildRounding   = 3;
  style.WindowRounding  = 3;
  style.ChildBorderSize = 1;
  style.FrameBorderSize = 1;
  style.PopupRounding   = 3;
  style.TabRounding     = 3;
  style.TabBorderSize   = 1;
  auto& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.Fonts->Clear();
  io.Fonts->AddFontFromFileTTF("NovaMono-Regular.ttf", 15.0f * scale);
  if (!ImGui::SFML::UpdateFontTexture()) {
    std::fprintf(stderr, "[DILLOXL]: Errore di update.\n");
    return 1;
  }

  dilloxl::TelloCommunication tellocom;
  dilloxl::TelloDrone drone{ tellocom };
  dilloxl::UserProgram::Configure(argc, argv);
  dilloxl::UserProgram program{"Studente"};

  dilloxl::GuiConsole ui_Console;
  dilloxl::GuiControl ui_Control;
  dilloxl::GuiEditor ui_Editor;
  ui_Editor.setSource(program.source());

  bool bShowExitDialog = false, bQuitApp = false;
  sf::Clock deltaClock;
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      if (event->is<sf::Event::Closed>()) {
        bShowExitDialog = true;
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());
    ImGui::SFML::SetCurrentWindow(window);
    ImGui::DockSpaceOverViewport(0, 0
      , ImGuiDockNodeFlags_PassthruCentralNode);
    tellocom.execute();

    const char* pTitleExitDialog = "Domanda Importante";
    if (bShowExitDialog) { ImGui::OpenPopup(pTitleExitDialog); }
    dilloxl::ShowModalDialog(pTitleExitDialog
      , "Sei sicuro di voler chiudere ?", &bShowExitDialog
      , [&]() { bShowExitDialog = false; bQuitApp = true;  }
      , [&]() { bShowExitDialog = false; bQuitApp = false; });
    if (bQuitApp) { window.close(); }

     ui_Editor.draw();
    ui_Control.draw();
    ui_Console.draw();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
