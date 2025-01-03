/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LEZIONI CXX
 * (C) 2024 Copyright by Michele Iacobellis
 * A project for students...
 * 
 * This file is part of LezioniCxx.
 *
 * LezioniCxx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * LezioniCxx is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with LezioniCxx. If not, see <http://www.gnu.org/licenses/>.
 * 
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef lezionicxx_system_h
#define lezionicxx_system_h

#include "lezionicxx_priv.h"
#include <chrono>

#define LEZIONICXX_CAPTURE_CPU(a,b)\
  if (a) { \
    std::fprintf(stderr,"BUG: %s (%s)\n", #a, b);\
    for(;;) { \
      std::this_thread::sleep_for(std::chrono::seconds(1)); \
    }\
  }

#define LEZIONICXX_TERM_RESETA "\x1b[0m"
#define LEZIONICXX_TERM_CLRSCR "\x1b[2J\x1b[3J\x1b[H"

#define LEZIONICXX_TERM_BLKBLK "\x1b[30;40m"
#define LEZIONICXX_TERM_REDBLK "\x1b[31;40m"
#define LEZIONICXX_TERM_GRNBLK "\x1b[32;40m"
#define LEZIONICXX_TERM_YLWBLK "\x1b[33;40m"
#define LEZIONICXX_TERM_BLUBLK "\x1b[34;40m"
#define LEZIONICXX_TERM_CYABLK "\x1b[36;40m"
#define LEZIONICXX_TERM_GRYBLK "\x1b[37;40m"

#define LEZIONICXX_TERM_BLKRED "\x1b[30;41m"
#define LEZIONICXX_TERM_REDRED "\x1b[31;41m"
#define LEZIONICXX_TERM_GRNRED "\x1b[32;41m"
#define LEZIONICXX_TERM_YLWRED "\x1b[33;41m"
#define LEZIONICXX_TERM_BLURED "\x1b[34;41m"
#define LEZIONICXX_TERM_CYARED "\x1b[36;41m"
#define LEZIONICXX_TERM_GRYRED "\x1b[37;41m"

#define LEZIONICXX_TERM_BLKGRN "\x1b[30;42m"
#define LEZIONICXX_TERM_REDGRN "\x1b[31;42m"
#define LEZIONICXX_TERM_GRNGRN "\x1b[32;42m"
#define LEZIONICXX_TERM_YLWGRN "\x1b[33;42m"
#define LEZIONICXX_TERM_BLUGRN "\x1b[34;42m"
#define LEZIONICXX_TERM_CYAGRN "\x1b[36;42m"
#define LEZIONICXX_TERM_GRYGRN "\x1b[37;42m"

#define LEZIONICXX_TERM_BLKYLW "\x1b[30;43m"
#define LEZIONICXX_TERM_REDYLW "\x1b[31;43m"
#define LEZIONICXX_TERM_GRNYLW "\x1b[32;43m"
#define LEZIONICXX_TERM_YLWYLW "\x1b[33;43m"
#define LEZIONICXX_TERM_BLUYLW "\x1b[34;43m"
#define LEZIONICXX_TERM_CYAYLW "\x1b[36;43m"
#define LEZIONICXX_TERM_GRYYLW "\x1b[37;43m"

#define LEZIONICXX_TERM_BLKBLU "\x1b[30;44m"
#define LEZIONICXX_TERM_REDBLU "\x1b[31;44m"
#define LEZIONICXX_TERM_GRNBLU "\x1b[32;44m"
#define LEZIONICXX_TERM_YLWBLU "\x1b[33;44m"
#define LEZIONICXX_TERM_BLUBLU "\x1b[34;44m"
#define LEZIONICXX_TERM_CYABLU "\x1b[36;44m"
#define LEZIONICXX_TERM_GRYBLU "\x1b[37;44m"

#define LEZIONICXX_TERM_BLKMAG "\x1b[30;45m"
#define LEZIONICXX_TERM_REDMAG "\x1b[31;45m"
#define LEZIONICXX_TERM_GRNMAG "\x1b[32;45m"
#define LEZIONICXX_TERM_YLWMAG "\x1b[33;45m"
#define LEZIONICXX_TERM_BLUMAG "\x1b[34;45m"
#define LEZIONICXX_TERM_CYAMAG "\x1b[36;45m"
#define LEZIONICXX_TERM_GRYMAG "\x1b[37;45m"

#define LEZIONICXX_TERM_BLKCYA "\x1b[30;46m"
#define LEZIONICXX_TERM_REDCYA "\x1b[31;46m"
#define LEZIONICXX_TERM_GRNCYA "\x1b[32;46m"
#define LEZIONICXX_TERM_YLWCYA "\x1b[33;46m"
#define LEZIONICXX_TERM_BLUCYA "\x1b[34;46m"
#define LEZIONICXX_TERM_CYACYA "\x1b[36;46m"
#define LEZIONICXX_TERM_GRYCYA "\x1b[37;46m"

#define LEZIONICXX_TERM_BLKWHT "\x1b[30;47m"
#define LEZIONICXX_TERM_REDWHT "\x1b[31;47m"
#define LEZIONICXX_TERM_GRNWHT "\x1b[32;47m"
#define LEZIONICXX_TERM_YLWWHT "\x1b[33;47m"
#define LEZIONICXX_TERM_BLUWHT "\x1b[34;47m"
#define LEZIONICXX_TERM_CYAWHT "\x1b[36;47m"
#define LEZIONICXX_TERM_GRYWHT "\x1b[37;47m"

#define LEZIONICXX_TERM_FGDIMM "\x1b[37;2;40m"
#define LEZIONICXX_TERM_FGBOLD "\x1b[37;1;40m"

namespace lezionicxx {
  std::vector<std::string> split(const std::string& s, std::string delimiter);
  std::string trim(const std::string&);
  void dump_data(const std::string& title, const uint8_t*, size_t);
}

namespace lezionicxx {
  struct Timer {
    Timer();
    std::chrono::duration<int64_t> elapsed();
    std::chrono::time_point<std::chrono::system_clock> t0;    
  };
}

namespace lezionicxx {
  bool ShowModalDialog(const std::string& title
    , const std::string& text
    , bool* bshow
    , const std::function<void()>& onOK
    , const std::function<void()>& onCancel = {});
}

#endif // lezionicxx_system_h
