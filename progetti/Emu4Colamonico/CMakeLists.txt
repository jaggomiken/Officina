# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# EMULATORE PER COLAMONICO
#   AUTORE: Michele Iacobellis
#     DATA: 2023-01-30
# VERSIONE: 1.0
# ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

cmake_minimum_required(VERSION 3.16)
project(Emu4Colamonico)

if (WIN32)
  set(SFML_DIR "D:/Lavori/ColamonicoSDK/lib/cmake/SFML")
  set(KEYSTONE_DIR "D:/Lavori/ColamonicoSDK")
  set(UNICORN2_DIR "D:/Lavori/ColamonicoSDK")
  set(COLAMONC_DIR "D:/Lavori/ColamonicoSDK")
else()
  set(SFML_DIR "${CMAKE_PREFIX_PATH}/lib/cmake/SFML")
  set(KEYSTONE_DIR "${CMAKE_PREFIX_PATH}")
  set(UNICORN2_DIR "${CMAKE_PREFIX_PATH}")
  set(COLAMONC_DIR "${CMAKE_PREFIX_PATH}")
endif()

include(FindThreads)
find_package(SFML 3.0 COMPONENTS Network Audio Graphics Window System REQUIRED)
find_package(OpenGL REQUIRED)
if (WIN32)
  set(keystone "${KEYSTONE_DIR}/lib/keystone.lib")
  set(unicorn2 "${UNICORN2_DIR}/lib/unicorn-import.lib")
else()
  set(keystone "${KEYSTONE_DIR}/lib/libkeystone.so")
  set(unicorn2 "${UNICORN2_DIR}/lib/libunicorn.so")
endif()

set (detected_libs 
  ${OPENGL_LIBRARIES}
  ${keystone}
  ${unicorn2}
  SFML::Network SFML::Audio SFML::Graphics
  Threads::Threads
)
set(detected_incs
  ${KEYSTONE_DIR}/include
  ${UNICORN2_DIR}/include
  ${COLAMONC_DIR}/include
)

set(target Emu4Colamonico)
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
  src/emu4cola_incs.h
  src/emu4cola_wrapper_unicorn.h
  src/emu4cola_wrapper_keystone.h
  src/emu4cola_context.h
  src/emu4cola_source_file.h
  src/emu4cola_unicorn_hooks.h
  src/emu4cola_extension.h
  src/emu4cola_gui_commands.h
  src/emu4cola_gui_editor.h
  src/emu4cola_gui_memory.h
  src/emu4cola_gui_ports.h
  src/emu4cola_gui_registers.h
  src/emu4cola_wrapper_unicorn.cpp
  src/emu4cola_wrapper_keystone.cpp
  src/emu4cola_context.cpp
  src/emu4cola_source_file.cpp
  src/emu4cola_unicorn_hooks.cpp
  src/emu4cola_extension.cpp
  src/emu4cola_gui_commands.cpp
  src/emu4cola_gui_editor.cpp
  src/emu4cola_gui_memory.cpp
  src/emu4cola_gui_ports.cpp
  src/emu4cola_gui_registers.cpp
  src/emu4cola_main.cpp
)

target_include_directories(${target} PUBLIC src src/ext ${detected_incs})
target_link_libraries(${target} LINK_PUBLIC ${libs})
set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
