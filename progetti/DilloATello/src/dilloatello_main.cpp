/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */
#include "dilloatello_sistema.h"

int main(int argc, char* argv[])
{
  // Costruisce la finestra di rendering dell'applicazione
  sf::RenderWindow mainWindow(sf::VideoMode({ 1280, 720 })
    , "DILLO A TELLO, del Prof. Michele Iacobellis - COLAMONICO");
  mainWindow.setFramerateLimit(60);
  ImGui::SFML::Init(mainWindow);

  // Costruisce i componenti fondamentali della GUI
  // TODO: ---

  // Configura lo stile dell'applicazione
  auto& style = ImGui::GetStyle();
  style.FrameRounding = 3;
  style.ChildRounding = 3;
  style.WindowRounding = 3;
  style.ChildBorderSize = 1;
  style.FrameBorderSize = 1;
  style.PopupRounding = 3;
  style.TabRounding = 3;
  auto& io = ImGui::GetIO();
  io.FontDefault = io.Fonts->Fonts[3];

  // Variabile di controllo ciclo
  uint32_t uExitLoop = 0;

  // Entra nel ciclo principale per gli eventi e il rendering
  sf::Clock deltaClock;
  while (mainWindow.isOpen() && (0 == uExitLoop)) {
    // Elabora gli eventi
    sf::Event event;
    while (mainWindow.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(mainWindow, event);
      if (event.type == sf::Event::Closed) { uExitLoop = 1; }
    }

    if (0 == uExitLoop)
    {
      // Aggiorna la finestra
      const sf::Time dt = deltaClock.restart();
      ImGui::SFML::Update(mainWindow, dt);

      // Disegna gli elementi dell'applicazione nella finestra corrente
      ImGui::SFML::SetCurrentWindow(mainWindow);
      ImGui::DockSpaceOverViewport(nullptr, ImGuiDockNodeFlags_PassthruCentralNode);
      // TODO: ---

      // Qui si pu� disegnare qualcosa (SOLO DIMOSTRATIVO)
      sf::CircleShape shape(100.f);
      shape.setFillColor(sf::Color::Green);
      mainWindow.clear();
      mainWindow.draw(shape);

      // Avvia il rendering
      ImGui::SFML::Render(mainWindow);
      mainWindow.display();
    }
  }

  mainWindow.close();
  ImGui::SFML::Shutdown();
  return 0;
}
