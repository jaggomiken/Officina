/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_user_program_h
#define dilloxl_user_program_h

#include "dilloxl_system.h"

namespace dilloxl {
  struct UserProgram {
    explicit UserProgram(const std::string&);
    UserProgram(const UserProgram&) = delete;
    UserProgram(UserProgram&&) noexcept;
    UserProgram& operator=(const UserProgram&) = delete;
    UserProgram& operator=(UserProgram&&) noexcept;
   ~UserProgram();

    void build();
    void run();

  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_user_program_h
