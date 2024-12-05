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
  bool checkchangedandwrite(float v) {
    if (fValue == v) { uChanged = 0;             }
    else             { uChanged = 1; fValue = v; }
    return (1 == uChanged);
  }

  std::string name;
  std::string labl;
     uint32_t uChanged;
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
class dilloxl::GuiControl::Impl {
public:
  Impl();
 ~Impl();
  void draw();

  std::thread m_threadTello;
  std::atomic_size_t m_szThreadStarted;
  size_t m_szNStatusPktsLast;
  bool m_bShowModal;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiControl::GuiControl()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiControl::GuiControl(GuiControl&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiControl& dilloxl::GuiControl::operator=(GuiControl&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiControl::~GuiControl()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiControl::draw()
{
  m_pImpl->draw();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiControl::Impl::Impl()
  : m_szThreadStarted   {     0 }
  , m_szNStatusPktsLast {     0 }
  , m_bShowModal        { false }
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiControl::Impl::~Impl()
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
void dilloxl::GuiControl::Impl::draw()
{
  auto& drone = TelloDrone::Get();
  const auto& st = drone.lastStatus();
  gvPropDescs[ 0].checkchangedandwrite(st.u.status.pitch);
  gvPropDescs[ 1].checkchangedandwrite(st.u.status.roll);
  gvPropDescs[ 2].checkchangedandwrite(st.u.status.yaw);
  gvPropDescs[ 3].checkchangedandwrite(st.u.status.speed_x);
  gvPropDescs[ 4].checkchangedandwrite(st.u.status.speed_y);
  gvPropDescs[ 5].checkchangedandwrite(st.u.status.speed_z);
  gvPropDescs[ 6].checkchangedandwrite(st.u.status.temperature_lo);
  gvPropDescs[ 7].checkchangedandwrite(st.u.status.temperature_hi);
  gvPropDescs[ 8].checkchangedandwrite(st.u.status.time_of_flight);
  gvPropDescs[ 9].checkchangedandwrite(st.u.status.height_from_floor);
  gvPropDescs[10].checkchangedandwrite(st.u.status.battery_level);
  gvPropDescs[11].checkchangedandwrite(st.u.status.barometer_cm);
  gvPropDescs[12].checkchangedandwrite(st.u.status.motor_time);
  gvPropDescs[13].checkchangedandwrite(st.u.status.acceleration_x);
  gvPropDescs[14].checkchangedandwrite(st.u.status.acceleration_y);
  gvPropDescs[15].checkchangedandwrite(st.u.status.acceleration_z);
  gvPropDescs[16].checkchangedandwrite(st.sequence);

  bool bExecProgram = false;
  if (ImGui::Begin("Comandi & Stato")) {
    if (ImGui::Button("Connetti")) { drone.com().tryLink(); }
    ImGui::SameLine();
    if (ImGui::Button( "Decolla")) { drone.takeoff(); }
    ImGui::SameLine();
    if (ImGui::Button( "Atterra")) { drone.land(); }
    ImGui::SameLine();
    if (ImGui::Button(   "Reset")) { drone.reset(); }
    ImGui::Separator();
    ImGui::Text("STATO: %-16s | ", drone.com().lastStatus().c_str());
    ImGui::SameLine();
    if (drone.com().isLinkAlive()) {
      ImGui::TextColored(ImVec4{ 0, 1.0, 0, 1.0 }
        , "%8s", "CONNESSO");
        m_szNStatusPktsLast = drone.com().nStatusPkts();
    } else {
      ImGui::TextColored(ImVec4{ 1.0, 0, 0, 1.0 }
        , "%8s", "ASSENTE");
    }
    ImGui::Separator();
    ImGui::Text("ESITO ULTIMO COMANDO: %s"
      , drone.lastCommandResult().c_str());
    ImGui::Separator();
    ImGui::Text(" N. Pacchetti Controllo In: %zu"
      , drone.com().nCtrlPktsIn());
    ImGui::Text("N. Pacchetti Controllo Out: %zu"
      , drone.com().nCtrlPktsOut());
    ImGui::Text("        N. Pacchetti Stato: %zu"
      , drone.com().nStatusPkts());
    ImGui::Text("        N. Pacchetti Video: %zu"
      , drone.com().nVideoPkts());
    ImGui::Separator();
    ImGui::Text("%s", drone.com().isWaitingForResponse()
      ? "In attesa di risposta..." : "Risposta ricevuta.");
    ImGui::Separator();
    ImGui::Text("ERRORE: %s", drone.com().lastError().c_str());
    ImGui::Separator();
    ImGui::TextUnformatted("IL TUO PROGRAMMA");
    ImGui::Separator();

    bool bDis = false;
    if (0 != m_szThreadStarted) { ImGui::BeginDisabled(); bDis = true; }
    if (ImGui::Button("Controlla")) {
      UserProgram::GetCurrent().build();
    }
    ImGui::SameLine();
    if (1 == m_szThreadStarted) { ImGui::BeginDisabled(); }
    if (ImGui::Button("Esegui")) { m_bShowModal = true; }
    if (1 == m_szThreadStarted) { ImGui::EndDisabled(); }
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
    if (ImGui::Button("Pulisci")) { 
      std::fprintf(stderr, "%s", DILLOXL_TERM_CLRSCR); 
    }
    ImGui::Separator();
    ImGui::Text("STATO PROGRAMMA: ");
    ImGui::SameLine();
    if (0 != m_szThreadStarted) {
      ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(0, 240, 0, 255));
      ImGui::Text("ESECUZIONE");
      ImGui::PopStyleColor();
    }
    else {
      ImGui::Text("SPENTO");
    }
    ImGui::Separator();
    ImGui::TextUnformatted("STATO DEL DRONE TELLO");
    ImGui::Separator();
    for (const auto& item : gvPropDescs) {
      if (1 == item.uChanged) {
        ImGui::TextColored({1.0, 1.0, 1.0, 1.0 }, "> %+8.2f", item.fValue);
        ImGui::SameLine();
        ImGui::TextColored({1.0, 1.0, 1.0, 1.0 }, "%s <", item.labl.c_str());
      } else {
        ImGui::TextColored({0.7, 0.7, 0.7, 1.0 }, "  %+8.2f", item.fValue);
        ImGui::SameLine();
        ImGui::TextColored({0.7, 0.7, 0.7, 1.0 }, "%s  ", item.labl.c_str());
      }
    }
    ImGui::End();
  }

  const char* pTitleExitDialog = "Domanda Importante##1";
  if (m_bShowModal) { ImGui::OpenPopup(pTitleExitDialog); }
  dilloxl::ShowModalDialog(pTitleExitDialog
    , "Sei sicuro di voler eseguire il tuo programma ?", &m_bShowModal
    , [&]() {
    m_bShowModal = false;
    bExecProgram = true;
  }, [&]() {
    m_bShowModal = false;
    bExecProgram = false;
  });

  if (bExecProgram) {
    bExecProgram = false;
    if (m_threadTello.joinable()) { m_threadTello.join(); }
    m_threadTello = std::thread([=]() {
      m_szThreadStarted = 1;
      //////////////////////////////////////////////////////////////////
      UserProgram::GetCurrent().run();
      //////////////////////////////////////////////////////////////////
      m_szThreadStarted = 0;
    });
  }
}
