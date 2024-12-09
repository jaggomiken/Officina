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

#define DILLOXL_TERM_BLKBLK "\x1b[30;40m"
#define DILLOXL_TERM_REDBLK "\x1b[31;40m"
#define DILLOXL_TERM_GRNBLK "\x1b[32;40m"
#define DILLOXL_TERM_YLWBLK "\x1b[33;40m"
#define DILLOXL_TERM_BLUBLK "\x1b[34;40m"
#define DILLOXL_TERM_CYABLK "\x1b[36;40m"
#define DILLOXL_TERM_GRYBLK "\x1b[37;40m"

#define DILLOXL_TERM_BLKRED "\x1b[30;41m"
#define DILLOXL_TERM_REDRED "\x1b[31;41m"
#define DILLOXL_TERM_GRNRED "\x1b[32;41m"
#define DILLOXL_TERM_YLWRED "\x1b[33;41m"
#define DILLOXL_TERM_BLURED "\x1b[34;41m"
#define DILLOXL_TERM_CYARED "\x1b[36;41m"
#define DILLOXL_TERM_GRYRED "\x1b[37;41m"

#define DILLOXL_TERM_BLKGRN "\x1b[30;42m"
#define DILLOXL_TERM_REDGRN "\x1b[31;42m"
#define DILLOXL_TERM_GRNGRN "\x1b[32;42m"
#define DILLOXL_TERM_YLWGRN "\x1b[33;42m"
#define DILLOXL_TERM_BLUGRN "\x1b[34;42m"
#define DILLOXL_TERM_CYAGRN "\x1b[36;42m"
#define DILLOXL_TERM_GRYGRN "\x1b[37;42m"

#define DILLOXL_TERM_BLKYLW "\x1b[30;43m"
#define DILLOXL_TERM_REDYLW "\x1b[31;43m"
#define DILLOXL_TERM_GRNYLW "\x1b[32;43m"
#define DILLOXL_TERM_YLWYLW "\x1b[33;43m"
#define DILLOXL_TERM_BLUYLW "\x1b[34;43m"
#define DILLOXL_TERM_CYAYLW "\x1b[36;43m"
#define DILLOXL_TERM_GRYYLW "\x1b[37;43m"

#define DILLOXL_TERM_BLKBLU "\x1b[30;44m"
#define DILLOXL_TERM_REDBLU "\x1b[31;44m"
#define DILLOXL_TERM_GRNBLU "\x1b[32;44m"
#define DILLOXL_TERM_YLWBLU "\x1b[33;44m"
#define DILLOXL_TERM_BLUBLU "\x1b[34;44m"
#define DILLOXL_TERM_CYABLU "\x1b[36;44m"
#define DILLOXL_TERM_GRYBLU "\x1b[37;44m"

#define DILLOXL_TERM_BLKMAG "\x1b[30;45m"
#define DILLOXL_TERM_REDMAG "\x1b[31;45m"
#define DILLOXL_TERM_GRNMAG "\x1b[32;45m"
#define DILLOXL_TERM_YLWMAG "\x1b[33;45m"
#define DILLOXL_TERM_BLUMAG "\x1b[34;45m"
#define DILLOXL_TERM_CYAMAG "\x1b[36;45m"
#define DILLOXL_TERM_GRYMAG "\x1b[37;45m"

#define DILLOXL_TERM_BLKCYA "\x1b[30;46m"
#define DILLOXL_TERM_REDCYA "\x1b[31;46m"
#define DILLOXL_TERM_GRNCYA "\x1b[32;46m"
#define DILLOXL_TERM_YLWCYA "\x1b[33;46m"
#define DILLOXL_TERM_BLUCYA "\x1b[34;46m"
#define DILLOXL_TERM_CYACYA "\x1b[36;46m"
#define DILLOXL_TERM_GRYCYA "\x1b[37;46m"

#define DILLOXL_TERM_BLKWHT "\x1b[30;47m"
#define DILLOXL_TERM_REDWHT "\x1b[31;47m"
#define DILLOXL_TERM_GRNWHT "\x1b[32;47m"
#define DILLOXL_TERM_YLWWHT "\x1b[33;47m"
#define DILLOXL_TERM_BLUWHT "\x1b[34;47m"
#define DILLOXL_TERM_CYAWHT "\x1b[36;47m"
#define DILLOXL_TERM_GRYWHT "\x1b[37;47m"

#define DILLOXL_TERM_FGDIMM "\x1b[37;2;40m"
#define DILLOXL_TERM_FGBOLD "\x1b[37;1;40m"

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
