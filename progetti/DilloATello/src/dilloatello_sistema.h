/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */
#ifndef sistema_h
#define sistema_h

#include <thread>

#include "imgui.h"
#include "imgui-SFML.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include "imgui_memory_editor.h"
#include "TextEditor.h"

#define DILLO_CAPTURE_CPU(a,b)\
  if (a) { \
    std::fprintf(stderr,"BUG: %s (%s)\n", #a, b);\
    for(;;) { \
      std::this_thread::sleep_for(std::chrono::seconds(1)); \
    }\
  }

void cancella_terminale();
void stampa(const char*, ...);
void stampa_stato(const char*, ...);
void stampa_errore(const char*, ...);
std::string scoda_prossimo_messaggio();

#endif // sistema_h
