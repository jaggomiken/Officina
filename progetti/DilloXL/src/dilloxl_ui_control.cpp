/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_ui_control.h"
#include "dilloxl_tello_communication.h"
#include "dilloxl_tello_drone.h"
#include "dilloxl_user_program.h"
#include <imgui.h>
#include <atomic>
#include <thread>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * STRUCT
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
struct PropDesc {
  std::string name;
  std::string labl;
  uint32_t uType;
  float fValue;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * TABLE
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
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
  , {         "sequence",              "Sequenza", 1, 0.0f }
};

 /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * CLASS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::GuiCommands::Impl {
public:
  Impl();
  ~Impl();
  void draw();

  std::thread m_threadTello;
  std::atomic_size_t m_szThreadStarted;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiCommands::GuiCommands()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiCommands::GuiCommands(GuiCommands&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiCommands& dilloxl::GuiCommands::operator=(GuiCommands&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiCommands::~GuiCommands()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiCommands::draw()
{
  m_pImpl->draw();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiCommands::Impl::Impl()
  : m_szThreadStarted { 0 }
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiCommands::Impl::~Impl()
{
  if (0 != m_szThreadStarted) {
    if (m_threadTello.joinable()) {
      m_threadTello.join();
    }
  }
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiCommands::Impl::draw()
{
  auto& drone = TelloDrone::Get();
  const auto& st = drone.lastStatus();
  gvPropDescs[ 0].fValue = st.u.status.pitch;
  gvPropDescs[ 1].fValue = st.u.status.roll;
  gvPropDescs[ 2].fValue = st.u.status.yaw;
  gvPropDescs[ 3].fValue = st.u.status.speed_x;
  gvPropDescs[ 4].fValue = st.u.status.speed_y;
  gvPropDescs[ 5].fValue = st.u.status.speed_z;
  gvPropDescs[ 6].fValue = st.u.status.temperature_lo;
  gvPropDescs[ 7].fValue = st.u.status.temperature_hi;
  gvPropDescs[ 8].fValue = st.u.status.time_of_flight;
  gvPropDescs[ 9].fValue = st.u.status.height_from_floor;
  gvPropDescs[10].fValue = st.u.status.battery_level;
  gvPropDescs[11].fValue = st.u.status.barometer_cm;
  gvPropDescs[12].fValue = st.u.status.motor_time;
  gvPropDescs[13].fValue = st.u.status.acceleration_x;
  gvPropDescs[14].fValue = st.u.status.acceleration_y;
  gvPropDescs[15].fValue = st.u.status.acceleration_z;
  gvPropDescs[16].fValue = st.sequence;

  if (ImGui::Begin("Comandi & Stato")) {
    bool bDis = false;
    if (0 != m_szThreadStarted) { ImGui::BeginDisabled(); bDis = true; }
    if (ImGui::Button("Avvia")) {
      if (m_threadTello.joinable()) { m_threadTello.join(); }
      m_threadTello = std::thread([=]() {
        m_szThreadStarted = 1;
        //////////////////////////////////////////////////////////////////
        std::this_thread::sleep_for(std::chrono::seconds(20));
        //////////////////////////////////////////////////////////////////
        m_szThreadStarted = 0;
      });
    }
    if (bDis) { ImGui::EndDisabled(); }
    ImGui::SameLine();
    
    bool bTelloDis = false;
    if (!drone.isActive()) { ImGui::BeginDisabled(); bTelloDis = true; }
    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(240, 0, 0, 255));
    if (ImGui::Button("Emergenza")) {
        drone.emergency();
    }
    if (bTelloDis) { ImGui::EndDisabled(); }
    ImGui::PopStyleColor();
    ImGui::SameLine();
    ImGui::Text("STATO: ");
    ImGui::SameLine();
    if (0 != m_szThreadStarted) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 240, 0, 255));
      ImGui::Text("ACCESO");
      ImGui::PopStyleColor();
    }
    else {
      ImGui::Text("SPENTO");
    }
    ImGui::Separator();
    for (const auto& item : gvPropDescs) {
      ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
      ImGui::TextColored({}, "%.2f", item.fValue);
      ImGui::SameLine();
      ImGui::TextColored({}, "%s", item.labl.c_str());
    }
    ImGui::End();
  }
}
