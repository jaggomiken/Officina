cmake_minimum_required(VERSION 3.16)
project(DilloATello)

include(FindThreads)
set (detected_libs Threads::Threads)

set(target DilloATello)
set(libs ${detected_libs})

add_executable(${target}
  src/main.cpp
  src/sistema.cpp
  src/sistema.h
  src/dillo_linguaggio_didattico.h
  src/tello.h
  src/UDPSocket.h
)

target_link_libraries(${target} LINK_PUBLIC ${libs})
set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
