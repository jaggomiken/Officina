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

#define DILLOXL_TERM_RESETA "\x1b[0m"
#define DILLOXL_TERM_CLRSCR "\x1b[2J\x1b[3J\x1b[H"
#define DILLOXL_TERM_BLKWHT "\x1b[30;40m"
#define DILLOXL_TERM_REDWHT "\x1b[31;40m"
#define DILLOXL_TERM_GRNWHT "\x1b[32;40m"
#define DILLOXL_TERM_BLUWHT "\x1b[34;40m"
#define DILLOXL_TERM_CYAWHT "\x1b[36;40m"
#define DILLOXL_TERM_GRYBLK "\x1b[40;37m"

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

namespace dilloxl {
  bool ShowModalDialog(const std::string& title
    , const std::string& text
    , bool* bshow
    , const std::function<void()>& onOK
    , const std::function<void()>& onCancel = {});
}

#endif // dilloxl_system_h
