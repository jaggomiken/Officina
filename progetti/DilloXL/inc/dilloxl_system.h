/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_system_h
#define dilloxl_system_h

#include "dilloxl_priv.h"
#include <chrono>

#define DILLOXL_CAPTURE_CPU(a,b)\
  if (a) { \
    std::fprintf(stderr,"BUG: %s (%s)\n", #a, b);\
    for(;;) { \
      std::this_thread::sleep_for(std::chrono::seconds(1)); \
    }\
  }

namespace dilloxl {
  std::vector<std::string> split(const std::string& s, std::string delimiter);
  std::string trim(const std::string&);
  void dump_data(const std::string& title, const uint8_t*, size_t);
}

namespace dilloxl {
  struct Timer {
    Timer();
    std::chrono::duration<int64_t> elapsed();
    std::chrono::time_point<std::chrono::system_clock> t0;    
  };
}

#endif // dilloxl_system_h
