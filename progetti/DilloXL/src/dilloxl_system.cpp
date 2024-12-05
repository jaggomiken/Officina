/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_system.h"
#include <imgui.h>
#include <imgui-SFML.h>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::vector<std::string>
  dilloxl::split(const std::string& s, std::string delimiter) 
{
  size_t
      pos_start = 0
    , pos_end
    , delim_len = delimiter.length();

  std::string token;
  std::vector<std::string> ret;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    ret.push_back(token);
  }

  ret.push_back(s.substr(pos_start));
  return ret;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::trim(const std::string& s) 
{
  std::string ret = s;
  return ret.erase(s.find_last_not_of(" \n\r\t")+1);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::dump_data(const std::string& title
  , const uint8_t* pData, size_t szSizeInBytes)
{
  std::fprintf(stderr, "%s BEGIN:\n", title.c_str());
  for (size_t k = 0;k < szSizeInBytes;++k) {
    std::fprintf(stderr, "%c", pData[k]);  }
  std::fprintf(stderr, "\n%s END.\n", title.c_str());
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::Timer::Timer() 
: t0{ std::chrono::system_clock::now() }
{
  
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::chrono::duration<int64_t> dilloxl::Timer::elapsed() 
{
  auto now = std::chrono::system_clock::now();
  return std::chrono::duration_cast<std::chrono::seconds>(now - t0);
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * FUNCTION
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
bool dilloxl::ShowModalDialog(const std::string& title
  , const std::string& text
  , bool* bshow
  , const std::function<void()>& onOK
  , const std::function<void()>& onCancel)
{
  bool bRet = false;
  ImVec2 center = ImGui::GetMainViewport()->GetCenter();
  ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
  if (ImGui::BeginPopupModal(title.c_str(), bshow
      , ImGuiWindowFlags_AlwaysAutoResize
      | ImGuiWindowFlags_NoResize)) {
    ImGui::TextUnformatted(text.c_str());
    if (ImGui::Button("OK")) { 
      ImGui::CloseCurrentPopup(); 
      if (onOK) { onOK(); bRet = true; }
    }
    ImGui::SameLine();
    if (ImGui::Button("Annulla")) { 
      ImGui::CloseCurrentPopup(); 
      if (onCancel) { onCancel(); bRet = false; }
    }
    ImGui::EndPopup();
  }
  return bRet;
}
