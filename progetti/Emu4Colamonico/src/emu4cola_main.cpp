/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-01-30
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 

#include "emu4cola_ext.h"
#include "emu4cola_wrapper_keystone.h"
#include "emu4cola_gui_commands.h"
#include "emu4cola_gui_editor.h"
#include "emu4cola_gui_memory.h"
#include "emu4cola_gui_ports.h"
#include "emu4cola_gui_registers.h"

int main(int argc, char* argv[])
{
  /// DA RIMUOVERE DA QUI, E' SOLO PER TEST
  cola::KeystoneWrapper wrapper;
  auto res = wrapper.compile(
    "push ebp;"
    "mov ebp, esp;"
    "sub esp, 4;"
    "push edi;"
    "push esi;"

    "mov eax, [ebp + 8];"
    "mov esi, [ebp + 12];"
    "mov edi, [ebp + 16];"

    "mov[ebp - 4], edi;"
    "add[ebp - 4], esi;"
    "add eax, [ebp - 4];"

    "pop esi;"
    "pop edi;"
    "mov esp, ebp;"
    "pop ebp;"
    "ret"
  );
  std::fprintf(stderr, "%s\n", std::get<0>(res) ? "SUCCESS" : "FAILURE");
  const auto& vbin = std::get<1>(res);
  for (const uint8_t byte : vbin) {
    std::fprintf(stdout, "%02x ", byte);
  }

  std::fprintf(stdout, "\n");

  // Costruisce la finestra di rendering dell'applicazione
  sf::RenderWindow mainWindow(sf::VideoMode({ 1280, 720 })
    , "EMULATORE CPU, del Prof. Michele Iacobellis per COLAMONICO");
  mainWindow.setFramerateLimit(60);
  ImGui::SFML::Init(mainWindow);

  // Costruisce i componenti fondamentali della GUI
  cola::GuiCommands guiCommands;
  cola::GuiEditor guiEditor;
  cola::GuiRegistersView guiRegisters;
  cola::GuiMemoryView guiMemory;
  cola::GuiPortsAndIrqView guiPortsIrqView;

  // Configura lo stile dell'applicazione
  auto& style = ImGui::GetStyle();
  style.FrameRounding   = 3;
  style.ChildRounding   = 3;
  style.WindowRounding  = 3;
  style.ChildBorderSize = 1;
  style.FrameBorderSize = 1;
  style.PopupRounding   = 3;
  style.TabRounding     = 3;
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
      guiCommands.draw();
      guiEditor.draw();
      guiRegisters.draw();
      guiMemory.draw();
      guiPortsIrqView.draw();

      // Qui si può disegnare qualcosa (SOLO DIMOSTRATIVO)
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
