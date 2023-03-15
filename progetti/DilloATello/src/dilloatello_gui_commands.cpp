/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-02-01
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#include "dilloatello_gui_commands.h"
#include "dilloatello_tello.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
struct PropDesc {
  std::string name;
  std::string labl;
  uint32_t uType;
  float fValue;
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static std::vector<PropDesc> gvPropDescs{
    {            "pitch",                 "Pitch", 1, 0.0f }
  , {             "roll",                  "Roll", 1, 0.0f }
  , {              "yaw",                   "Yaw", 1, 0.0f }
  , {          "speed_x",    u8"Velocità Lungo X", 1, 0.0f }
  , {          "speed_y",    u8"Velocità Lungo Y", 1, 0.0f }
  , {          "speed_z",    u8"Velocità Lungo Z", 1, 0.0f }
  , {   "temperature_lo",       "Temperatura Max", 1, 0.0f }
  , {   "temperature_hi",       "Temperatura Min", 1, 0.0f }
  , {   "time_of_flight",         "Tempo di Volo", 1, 0.0f }
  , {"height_from_floor",      "Altezza da Terra", 1, 0.0f }
  , {    "battery_level",              "Batteria", 1, 0.0f }
  , {     "barometer_cm",             "Barometro", 1, 0.0f }
  , {       "motor_time",          "Tempo Motore", 1, 0.0f }
  , {   "acceleration_x", "Accelerazione Lungo X", 1, 0.0f }
  , {   "acceleration_y", "Accelerazione Lungo Y", 1, 0.0f }
  , {   "acceleration_z", "Accelerazione Lungo Z", 1, 0.0f }
};

 /* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  *
  * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
class dillo::GuiCommands::Impl {
public:
  Impl();
  ~Impl();
  void draw();

  std::thread m_threadTello;
  bool m_bThreadStarted;
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiCommands::GuiCommands()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLO_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiCommands::GuiCommands(GuiCommands&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiCommands& dillo::GuiCommands::operator=(GuiCommands&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiCommands::~GuiCommands()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void dillo::GuiCommands::draw()
{
  m_pImpl->draw();
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiCommands::Impl::Impl()
  : m_bThreadStarted { false }
{
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
dillo::GuiCommands::Impl::~Impl()
{

}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
void dillo::GuiCommands::Impl::draw()
{
  if (ImGui::Begin("Comandi & Stato")) {
    bool bDis = false;
    if (m_bThreadStarted) { ImGui::BeginDisabled(); bDis = true; }
    if (ImGui::Button("Avvia")) {
      m_bThreadStarted = true;
      m_threadTello = std::thread([]() {
        programma_tello();
      });
    }
    if (bDis) { ImGui::EndDisabled(); }
    ImGui::SameLine();
    if (ImGui::Button("Emergenza")) {

    }
    ImGui::SameLine();
    ImGui::Text("STATO: %s", m_bThreadStarted ? "ACCESO" : "SPENTO");
    ImGui::Separator();
    for (auto& item : gvPropDescs) {
      ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
      ImGui::InputFloat(item.labl.c_str()
        , &item.fValue, .0f, .0f, "%.2f", ImGuiInputTextFlags_ReadOnly);
    }
    ImGui::End();
  }
}
