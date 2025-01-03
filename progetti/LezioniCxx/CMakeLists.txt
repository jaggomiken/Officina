# /* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
#  * LEZIONI CXX
#  * (C) 2024 Copyright by Michele Iacobellis
#  * A project for students...
#  * 
#  * This file is part of LezioniCxx.
#  *
#  * LezioniCxx is free software: you can redistribute it and/or modify
#  * it under the terms of the GNU General Public License as published by
#  * the Free Software Foundation, either version 3 of the License, or
#  * (at your option) any later version.
#  *
#  * LezioniCxx is distributed in the hope that it will be useful,
#  * but WITHOUT ANY WARRANTY; without even the implied warranty of
#  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  * GNU General Public License for more details.
#  *
#  * You should have received a copy of the GNU General Public License
#  * along with LezioniCxx. If not, see <http://www.gnu.org/licenses/>.
#  * 
#  * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#
# Per il build è necessario specificare BUILD_SHARED_LIBS=OFF
# su cmake quando con MSYS/UCRT64 e quindi Windows.
#
cmake_minimum_required(VERSION 3.22)
project(lezionicxx VERSION 1.0.0 LANGUAGES CXX)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML COMPONENTS Graphics Window System Audio Network)
find_package(ImGui-SFML)
find_package(PkgConfig REQUIRED)
set(SRC 
    src/lezionicxx_main.cpp 
    src/lezionicxx_system.cpp 
)
set(INC 
    inc/lezionicxx_priv.h 
    inc/lezionicxx_system.h
)
set(EXT "")
add_executable(lezionicxx ${SRC} ${INC} ${EXT})
target_include_directories(lezionicxx PUBLIC inc ext)
target_link_libraries(lezionicxx PRIVATE ImGui-SFML::ImGui-SFML SFML::Audio SFML::Network)
target_compile_options(lezionicxx PRIVATE ${IMGUI_SFML_WARNINGS})
include(GNUInstallDirs)
install(TARGETS lezionicxx
    RUNTIME DESTINATION bin)
if (WIN32)
  install(FILES 
    fonts/NovaMono-Regular.ttf
    DESTINATION bin)
else()
  install(FILES 
    fonts/NovaMono-Regular.ttf
    DESTINATION bin)
endif()
