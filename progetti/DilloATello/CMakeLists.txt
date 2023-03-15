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

add_executable(${target}
  src/dilloatello_main.cpp
  src/dilloatello_sistema.cpp
  src/dilloatello_sistema.h
  src/dilloatello_dillo_linguaggio_didattico.h
  src/dilloatello_tello.h
  src/UDPSocket.h
)

target_link_libraries(${target} LINK_PUBLIC ${libs})
set_property(TARGET ${target} PROPERTY CXX_STANDARD 17)
