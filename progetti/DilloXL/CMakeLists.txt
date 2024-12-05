#
# Per il build è necessario specificare BUILD_SHARED_LIBS=OFF
# su cmake.
#
cmake_minimum_required(VERSION 3.22)
project(dilloxl VERSION 1.0.0 LANGUAGES CXX)
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML COMPONENTS Graphics Window System Audio Network)
find_package(ImGui-SFML)
set(SRC 
    src/dilloxl_main.cpp 
    src/dilloxl_system.cpp 
    src/dilloxl_tello_communication.cpp 
    src/dilloxl_tello_drone.cpp 
    src/dilloxl_ui_console.cpp 
    src/dilloxl_ui_control.cpp 
    src/dilloxl_ui_editor.cpp 
    src/dilloxl_user_program.cpp
    src/dilloxl_if.cpp
)
set(INC 
    inc/dilloxl_priv.h 
    inc/dilloxl_system.h
    inc/dilloxl_tello_communication.h 
    inc/dilloxl_tello_drone.h
    inc/dilloxl_ui_console.h 
    inc/dilloxl_ui_control.h 
    inc/dilloxl_ui_editor.h 
    inc/dilloxl_user_program.h
    inc/dilloxl_if.h
    inc/dilloxl
)
set(EXT ext/TextEditor.h ext/TextEditor.cpp)
add_executable(dilloxl ${SRC} ${INC} ${EXT})
target_include_directories(dilloxl PUBLIC inc ext)
target_link_libraries(dilloxl PRIVATE ImGui-SFML::ImGui-SFML SFML::Audio SFML::Network)
target_compile_options(dilloxl PRIVATE ${IMGUI_SFML_WARNINGS})
target_link_options(dilloxl PUBLIC "-Wl,--export-all-symbols")
include(GNUInstallDirs)
install(TARGETS dilloxl
    RUNTIME DESTINATION bin)
install(FILES inc/dilloxl ${CMAKE_BINARY_DIR}/libdilloxl.dll.a
    DESTINATION bin)