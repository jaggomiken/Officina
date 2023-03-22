# /* ==========================================================================
#  * TELLO, Progetto Didattico per Colamonico
#  * Autore: Michele Iacobellis
#  *   Data: 20221215
#  * ========================================================================== */

cmake_minimum_required(VERSION 3.16)
project(DilloATello)

if (WIN32)
  set(SFML_DIR "D:/Lavori/ColamonicoSDK/lib/cmake/SFML")
  set(COLAMONC_DIR "D:/Lavori/ColamonicoSDK")
else()
  set(SFML_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/SFML")
  set(COLAMONC_DIR "${CMAKE_PREFIX_PATH}")
endif()

include(FindThreads)
find_package(SFML 3.0 COMPONENTS Network Audio Graphics Window System REQUIRED)
find_package(OpenGL REQUIRED)

set (detected_libs 
  ${OPENGL_LIBRARIES}
  ${keystone}
  SFML::Network SFML::Audio SFML::Graphics
  Threads::Threads
)
set(detected_incs
  ${COLAMONC_DIR}/include
)

set(target DilloATello)
set(libs ${detected_libs})
set(ext_cpp
  src/ext/TextEditor.cpp
  src/ext/TextEditor.h
  src/ext/imconfig.h
  src/ext/imgui.cpp
  src/ext/imgui.h
  src/ext/imgui_demo.cpp
  src/ext/imgui_draw.cpp
  src/ext/imgui_impl_opengl2.cpp
  src/ext/imgui_impl_opengl2.h
  src/ext/imgui_impl_opengl3.cpp
  src/ext/imgui_impl_opengl3.h
  src/ext/imgui_impl_opengl3_loader.h
  src/ext/imgui_internal.h
  src/ext/imgui_memory_editor.h
  src/ext/imgui_tables.cpp
  src/ext/imgui_widgets.cpp
  src/ext/imstb_rectpack.h
  src/ext/imstb_textedit.h
  src/ext/imstb_truetype.h
  src/ext/imgui-SFML.h
  src/ext/imgui-SFML_export.h
  src/ext/imconfig-SFML.h
  src/ext/imgui-SFML.cpp
)

add_executable(${target} ${ext_cpp}
  src/dilloatello_main.cpp
  src/dilloatello_programmi.cpp
  src/dilloatello_sistema.cpp
  src/dilloatello_tello.cpp
  src/dilloatello_gui_commands.cpp
  src/dilloatello_gui_console.cpp
  src/dilloatello_sistema.h
  src/dilloatello_dillo_linguaggio_didattico.h
  src/dilloatello_tello.h
  src/dilloatello_gui_commands.h
  src/dilloatello_gui_console.h
  src/UDPsocket.h
)

target_include_directories(${target} PUBLIC src src/ext ${detected_incs})
target_link_libraries(${target} LINK_PUBLIC ${libs})
set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
if (MSVC)
	target_compile_options(${target} PUBLIC /utf-8)
endif()
