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

int main(int argc, char* argv[]) 
{
  sf::RenderWindow window(sf::VideoMode({ 1920, 1080 })
    , "DILLO XL Versione 1.0 - By Prof. Michele Iacobellis");

  window.setFramerateLimit(60);
  if (!ImGui::SFML::Init(window)) {
    std::fprintf(stderr, "[DILLOXL]: Errore di init.\n");
    return 1;
  }

  const float scale = 2.0f;
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

  dilloxl::GuiControl ui_Control;
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

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    if (ImGui::BeginPopupModal("Domanda Importante", &bShowExitDialog
        , ImGuiWindowFlags_AlwaysAutoResize
        | ImGuiWindowFlags_NoResize)) {
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

     ui_Editor.draw();
    ui_Control.draw();

    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
