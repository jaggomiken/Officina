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

#include <imgui.h>
#include <imgui-SFML.h>

int main() 
{
  const float scale = 2.0f;
  sf::RenderWindow window(sf::VideoMode({1920, 1080})
    , "DILLO XL - By Prof. Michele Iacobellis");

  window.setFramerateLimit(60);
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

  dilloxl::GuiCommands ui_Commands;
  dilloxl::GuiEditor ui_Editor;
  bool bShowExitDialog = false, bQuitApp = false;;

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
    ImGui::DockSpaceOverViewport(0, 0, ImGuiDockNodeFlags_PassthruCentralNode);
    tellocom.execute();

    if (bShowExitDialog) {
      ImGui::OpenPopup("Domanda Importante");
    }

    if (ImGui::BeginPopupModal("Domanda Importante", &bShowExitDialog)) {
      ImGui::TextUnformatted("Sei sicuro di voler chiudere ?");
      if (ImGui::Button(     "OK")) { 
        ImGui::CloseCurrentPopup(); 
        bQuitApp = true;
        bShowExitDialog = false;
      }
      ImGui::SameLine();
      if (ImGui::Button("Annulla")) { 
        ImGui::CloseCurrentPopup(); 
        bQuitApp = false; 
        bShowExitDialog = false;
      }
      ImGui::EndPopup();
    }

    if (bQuitApp) { window.close(); }

    ImGui::Begin("Centro Controllo Tello");
    if (ImGui::Button("Connetti")) { tellocom.tryLink(); }
    ImGui::SameLine();
    if (ImGui::Button("Decolla")) { drone.takeoff(); }
    ImGui::SameLine();
    if (ImGui::Button("Atterra")) { drone.land(); }
    ImGui::Text("STATO: %s | ", tellocom.lastStatus().c_str());
    ImGui::SameLine();
    ImGui::Text("N. Pacchetti Controllo in/out: %zu/%zu"
      , tellocom.nCtrlPktsIn(), tellocom.nCtrlPktsOut());
    ImGui::Text("N. Pacchetti Stato: %zu | "
      , tellocom.nStatusPkts());
    ImGui::SameLine();
    ImGui::Text("N. Pacchetti Video: %zu"
      , tellocom.nVideoPkts());
    ImGui::Text("ERRORE: %s", tellocom.lastError().c_str());
    ImGui::End();

    ui_Editor.draw();
    ui_Commands.draw();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
