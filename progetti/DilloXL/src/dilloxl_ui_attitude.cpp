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
#include <cmath>
#include "dilloxl_ui_attitude.h"
#include "dilloxl_tello_drone.h"
#include "imgui-SFML.h"
#include "imgui.h"

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * MACROS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define DILLOXL_UI_ATTITUDE_DEBUG                                          0

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static inline ImVec2 operator+(const ImVec2& lhs, const ImVec2& rhs) { 
  return ImVec2(lhs.x + rhs.x, lhs.y + rhs.y); 
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static inline ImVec2 ImRotate(const ImVec2& v, float cos_a, float sin_a) { 
  return ImVec2(v.x * cos_a - v.y * sin_a, v.x * sin_a + v.y * cos_a);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void ImageRotated(ImTextureID tex_id, ImVec2 center, ImVec2 size, float angle)
{
  ImDrawList* pDrawList = ImGui::GetWindowDrawList();

  float cos_a = ::cosf(angle);
  float sin_a = ::sinf(angle);
  ImVec2 pos[4] = {
    center + ImRotate(ImVec2(-size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
    center + ImRotate(ImVec2(+size.x * 0.5f, -size.y * 0.5f), cos_a, sin_a),
    center + ImRotate(ImVec2(+size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a),
    center + ImRotate(ImVec2(-size.x * 0.5f, +size.y * 0.5f), cos_a, sin_a)
  };
  ImVec2 uvs[4] = { 
    ImVec2(0.0f, 0.0f), 
    ImVec2(1.0f, 0.0f), 
    ImVec2(1.0f, 1.0f), 
    ImVec2(0.0f, 1.0f) 
  };
  pDrawList->AddImageQuad(tex_id
    , pos[0], pos[1], pos[2], pos[3]
    , uvs[0], uvs[1], uvs[2], uvs[3]
    , IM_COL32_WHITE);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DECLARATIONS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::GuiAttitude::Impl {
public:
  Impl();
 ~Impl();
  void draw();

  float m_fLastR;
  float m_fLastP;
  float m_fLastY;

  sf::Texture m_texTop;
  sf::Texture m_texSid;
  sf::Texture m_texFro;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiAttitude::GuiAttitude()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiAttitude::GuiAttitude(GuiAttitude&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiAttitude& dilloxl::GuiAttitude::operator=(GuiAttitude&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiAttitude::~GuiAttitude()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiAttitude::draw()
{
  m_pImpl->draw();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiAttitude::Impl::Impl()
  : m_texFro{ sf::Image{"drone_front.png" }}
  , m_texSid{ sf::Image{ "drone_side.png" }}
  , m_texTop{ sf::Image{  "drone_top.png" }}
  , m_fLastR{                             0}
  , m_fLastP{                             0}
  , m_fLastY{                             0}
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::GuiAttitude::Impl::~Impl()
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::GuiAttitude::Impl::draw()
{
  auto& drone = TelloDrone::Get();
  auto r = drone.lastStatus().u.status.roll;
  auto p = drone.lastStatus().u.status.pitch;
  auto y = drone.lastStatus().u.status.yaw;

	if (ImGui::Begin("Assetto Drone", nullptr, 0)) {
	  ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImDrawList* pDrawList = ImGui::GetWindowDrawList();
    float icon_w = 200.0f, icon_h = .0f, h_pad = 24.0f;;
    auto pos = ImGui::GetCursorScreenPos();
    { auto siz = m_texTop.getSize(); float aspect = float(siz.y) / float(siz.x);
      icon_h = icon_w * aspect;
      pDrawList->AddCircleFilled(ImVec2{ pos.x + siz.x / 2.0f, pos.y + siz.y / 2.0f }, siz.x / 2.0f, IM_COL32(120, 120, 120, 255), 0);
      ImageRotated(m_texTop.getNativeHandle()
        , ImVec2{ pos.x + siz.x / 2.0f, pos.y + siz.y / 2.0f }
        , ImVec2{ icon_w, icon_h }, ((90.0f + y) * M_PI) / 180.0f); }
    pos.x += icon_w + h_pad;
    { auto siz = m_texSid.getSize(); float aspect = float(siz.y) / float(siz.x);
      icon_h = icon_w * aspect;
      pDrawList->AddCircleFilled(ImVec2{ pos.x + siz.x / 2.0f, pos.y + siz.y / 2.0f }, siz.x / 2.0f, IM_COL32(120, 120, 120, 255), 0);
      ImageRotated(m_texSid.getNativeHandle()
        , ImVec2{ pos.x + siz.x / 2.0f, pos.y + siz.y / 2.0f }
        , ImVec2{ icon_w, icon_h }, (-p * M_PI) / 180.0f); }
    pos.x += icon_w + h_pad;
    { auto siz = m_texFro.getSize(); float aspect = float(siz.y) / float(siz.x);
      icon_h = icon_w * aspect;
      pDrawList->AddCircleFilled(ImVec2{ pos.x + siz.x / 2.0f, pos.y + siz.y / 2.0f }, siz.x / 2.0f, IM_COL32(120, 120, 120, 255), 0);
      ImageRotated(m_texFro.getNativeHandle()
          , ImVec2{ pos.x + siz.x / 2.0f, pos.y + siz.y / 2.0f }
          , ImVec2{ icon_w, icon_h }, (-r * M_PI) / 180.0f); }
  }
	ImGui::End();

  m_fLastP = p;
  m_fLastR = r;
  m_fLastY = y;
}
