# /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#  * DILLO EXTRA LARGE - DILLOXL
#  * (C) 2024 Copyright by Michele Iacobellis
#  * A project for students...
#  * 
#  * This file is part of DILLOXL.
#  *
#  * DILLOXL is free software: you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation, either version 3 of the License, or
#  * (at your option) any later version.
#  *
#  * DILLOXL is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with DILLOXL. If not, see <http://www.gnu.org/licenses/>.
#  * 
#  * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#
# Per il build è necessario specificare BUILD_SHARED_LIBS=OFF
# su cmake quando con MSYS/UCRT64 e quindi Windows.
#
cmake_minimum_required(VERSION 3.22)
project(dilloxl VERSION 1.0.0 LANGUAGES CXX)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML COMPONENTS Graphics Window System Audio Network)
find_package(ImGui-SFML)
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG libavcodec libavformat libavutil libswscale libswresample)
set(SRC 
    src/dilloxl_main.cpp 
    src/dilloxl_system.cpp 
    src/dilloxl_tello_communication.cpp 
    src/dilloxl_tello_drone.cpp 
    src/dilloxl_ui_console.cpp 
    src/dilloxl_ui_control.cpp 
    src/dilloxl_ui_editor.cpp 
    src/dilloxl_ui_video.cpp
    src/dilloxl_ui_attitude.cpp 
    src/dilloxl_user_program.cpp
    src/dilloxl_if.cpp
    src/dilloxl_video_decoder.cpp
)
set(INC 
    inc/dilloxl_priv.h 
    inc/dilloxl_system.h
    inc/dilloxl_tello_communication.h 
    inc/dilloxl_tello_drone.h
    inc/dilloxl_ui_console.h 
    inc/dilloxl_ui_control.h 
    inc/dilloxl_ui_editor.h 
    inc/dilloxl_ui_video.h
    inc/dilloxl_ui_attitude.h 
    inc/dilloxl_user_program.h
    inc/dilloxl_if.h
    inc/dilloxl_video_decoder.h
    inc/dilloxl
)
set(EXT ext/TextEditor.h ext/TextEditor.cpp)
add_executable(dilloxl ${SRC} ${INC} ${EXT})
target_include_directories(dilloxl PUBLIC inc ext ${FFMPEG_INCLUDE_DIRS})
target_link_libraries(dilloxl PRIVATE ImGui-SFML::ImGui-SFML SFML::Audio SFML::Network ${FFMPEG_LIBRARIES})
target_link_directories(dilloxl PRIVATE ${FFMPEG_LIBRARY_DIRS})
target_compile_options(dilloxl PRIVATE ${IMGUI_SFML_WARNINGS})
if (WIN32)
  target_link_options(dilloxl PUBLIC "-Wl,--export-all-symbols")
else()
  set_target_properties(dilloxl PROPERTIES OUTPUT_NAME "dilloxl-exec")
endif()
include(GNUInstallDirs)
install(TARGETS dilloxl
    RUNTIME DESTINATION bin)
if (WIN32)
  install(FILES 
    inc/dilloxl 
    fonts/NovaMono-Regular.ttf
    images/drone_front.png
    images/drone_side.png
    images/drone_top.png
    ${CMAKE_BINARY_DIR}/libdilloxl.dll.a
    DESTINATION bin)
else()
  install(FILES 
    inc/dilloxl 
    fonts/NovaMono-Regular.ttf
    images/drone_front.png
    images/drone_side.png
    images/drone_top.png
    DESTINATION bin)
endif()