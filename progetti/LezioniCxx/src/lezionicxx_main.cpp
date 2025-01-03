/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LEZIONI CXX
 * (C) 2024 Copyright by Michele Iacobellis
 * A project for students...
 * 
 * This file is part of LEZIONICXX.
 *
 * LEZIONICXX is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LEZIONICXX is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LEZIONICXX. If not, see <http://www.gnu.org/licenses/>.
 * 
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "lezionicxx_system.h"
#include <imgui.h>
#include <imgui-SFML.h>

int main(int argc, char* argv[]) 
{
  const char* pTitle = 
    "LEZIONI CXX Versione 0.0.0 - "
    "By Prof. Michele Iacobellis (COLAMONICO-CHIARULLI)";
  std::fprintf(stderr, "[LEZIONICXX]: %s\n", pTitle);
  std::fprintf(stderr, "[LEZIONICXX]: argv[1] = valore scala display.\n");
  std::fprintf(stderr, "[LEZIONICXX]: argv[2] = limite framerate.\n");
  std::fprintf(stderr, "[LEZIONICXX]: E' preferibile mettere 0 perche' il "
    "traffico di rete e' gestito nel ciclo della GUI.\n");

  const float scale = std::atof(nullptr == argv[1] ? "1.5" : argv[1]);
  std::fprintf(stderr, "[LEZIONICXX]:  Valore di scala: %f\n", scale);
  const uint32_t uFRL = std::atoi(nullptr == argv[2] ? "0" : argv[2]);
  std::fprintf(stderr, "[LEZIONICXX]: Limite framerate: %u\n", uFRL);

  sf::RenderWindow window(sf::VideoMode({ 1800, 1000 }), pTitle);
  window.setFramerateLimit(uFRL);
  if (!ImGui::SFML::Init(window)) {
    std::fprintf(stderr, "[LEZIONICXX]: Errore di init.\n");
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
    std::fprintf(stderr, "[LEZIONICXX]: Errore di update.\n");
    return 1;
  }

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

    const char* pTitleExitDialog = "Domanda Importante";
    if (bShowExitDialog) { ImGui::OpenPopup(pTitleExitDialog); }
    lezionicxx::ShowModalDialog(pTitleExitDialog
      , "Sei sicuro di voler chiudere ?", &bShowExitDialog
      , [&]() { bShowExitDialog = false; bQuitApp = true;  }
      , [&]() { bShowExitDialog = false; bQuitApp = false; });
    if (bQuitApp) { window.close(); }

    window.clear();
    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}
