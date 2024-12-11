/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) 2024 Copyright by Michele Iacobellis
 * A project for students...
 * 
 * This file is part of DILLOXL.
 *
 * DILLOXL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DILLOXL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DILLOXL. If not, see <http://www.gnu.org/licenses/>.
 * 
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_ui_console.h"
#include "dilloxl_tello_communication.h"
#include "dilloxl_tello_drone.h"
#include <imgui.h>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * CLASS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::GuiConsole::Impl {
public:
  Impl();
 ~Impl();
  void draw();

  bool m_bShowModal;
  int32_t m_iSxCm;
  int32_t m_iDxCm;
  int32_t m_iUpCm;
  int32_t m_iDnCm;
  int32_t m_iFwCm;
  int32_t m_iBwCm;
    float m_fCwDeg;
    float m_fCcDeg;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiConsole::GuiConsole()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiConsole::GuiConsole(GuiConsole&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiConsole& dilloxl::GuiConsole::operator=(GuiConsole&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiConsole::~GuiConsole()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiConsole::draw()
{
  m_pImpl->draw();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiConsole::Impl::Impl()
  : m_bShowModal { false }
  , m_iSxCm      {    50 }
  , m_iDxCm      {    50 }
  , m_iUpCm      {    50 }
  , m_iDnCm      {    50 }
  , m_iFwCm      {    50 }
  , m_iBwCm      {    50 }
  , m_fCwDeg     { 10.0f }
  , m_fCcDeg     { 10.0f }
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiConsole::Impl::~Impl()
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiConsole::Impl::draw()
{
  auto& drone = TelloDrone::Get();
  if (ImGui::Begin("Console")) {
    float fWL = ImGui::GetFontSize() * 3.4;
    float fWS = ImGui::GetFontSize() * 3;
    ImGui::TextUnformatted("CONTROLLO DIRETTO DEL DRONE");

    ImGui::Separator();
    ImGui::TextUnformatted("cm");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderInt("##1", &m_iSxCm, 20, 500);
    ImGui::SameLine();
    if (ImGui::Button("SX", { fWL, .0f })) { drone.move_left(float(m_iSxCm)); }
    ImGui::SameLine();
    if (ImGui::Button("DX", { fWL, .0f })) { drone.move_right(float(m_iDxCm)); }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderInt("##2", &m_iDxCm, 20, 500);
    ImGui::SameLine();
    ImGui::TextUnformatted("cm");

    ImGui::Separator();
    ImGui::TextUnformatted("cm");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderInt("##3", &m_iUpCm, 20, 500);
    ImGui::SameLine(); 
    if (ImGui::Button(   "Su",{ fWL, .0f })) { drone.move_up(float(m_iUpCm)); }
    ImGui::SameLine();
    if (ImGui::Button(u8"Giù",{ fWL, .0f })) { drone.move_down(float(m_iDnCm)); }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderInt("##4", &m_iDnCm, 20, 500);
    ImGui::SameLine();
    ImGui::TextUnformatted("cm");

    ImGui::Separator();
    ImGui::TextUnformatted("cm");
    ImGui::SameLine(); 
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderInt("##5", &m_iFwCm, 20, 500);
    ImGui::SameLine();
    if (ImGui::Button("Avanti",{ fWL, .0f })) { drone.forward(float(m_iFwCm)); }
    ImGui::SameLine();
    if (ImGui::Button("Indietro",{ fWL, .0f })) { drone.backward(float(m_iBwCm)); }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderInt("##6", &m_iBwCm, 20, 500);
    ImGui::SameLine();
    ImGui::TextUnformatted("cm");

    ImGui::Separator();
    ImGui::TextUnformatted(u8" °");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderFloat("##7", &m_fCwDeg, 1.0f, 360.0f);
    ImGui::SameLine(); 
    if (ImGui::Button("Rot CW",{ fWL, .0f })) { drone.rotate_cw(m_fCwDeg); }
    ImGui::SameLine();
    if (ImGui::Button("Rot CC",{ fWL, .0f })) { drone.rotate_cc(m_fCcDeg); }
    ImGui::SameLine();
    ImGui::SetNextItemWidth(fWS);
    ImGui::SliderFloat("##8", &m_fCcDeg, 1.0f, 360.0f);
    ImGui::SameLine();
    ImGui::TextUnformatted(u8"° ");

    ImGui::Separator();
    ImGui::TextUnformatted(">>");
    ImGui::SameLine();
    if (ImGui::Button("Stop", { fWL, .0f })) { drone.stop(); }    
    ImGui::SameLine();
    ImGui::TextUnformatted("<< Premi, per fermare il drone...");
  }
  ImGui::End();
}
