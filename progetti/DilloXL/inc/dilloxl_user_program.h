/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifndef dilloxl_user_program_h
#define dilloxl_user_program_h

#include "dilloxl_system.h"
#include "dilloxl"

namespace dilloxl {
  struct UserProgram {
    static void Configure(int32_t argc, char* argv[]);
    static UserProgram& GetCurrent();

    UserProgram(const std::string&);
    UserProgram(const UserProgram&) = delete;
    UserProgram(UserProgram&&) noexcept;
    UserProgram& operator=(const UserProgram&) = delete;
    UserProgram& operator=(UserProgram&&) noexcept;
   ~UserProgram();

    std::string name() const;
    void setSource(const std::string&);
    std::string source() const;

    void build();
    void run();

  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_user_program_h
