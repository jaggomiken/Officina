/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-01-30
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef emu4cola_incs_h
#define emu4cola_incs_h

#include <cstdio>
#include <cstdint>
#include <thread>
#include <string>
#include <mutex>
#include <new>
#include <tuple>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <chrono>

#define COLA_CAPTURE_CPU(condition, msg)\
  if (condition) { \
    std::fprintf(stderr, "CPU_CATTURATA: %s\n%s\n", #condition, msg); \
    for(;;) { std::this_thread::sleep_for(std::chrono::seconds(1)); } \
  }

#endif // emu4cola_incs_h