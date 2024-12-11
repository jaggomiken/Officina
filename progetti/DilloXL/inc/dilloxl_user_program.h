/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) 2024 Copyright by Michele Iacobellis
 * A project for students...
 * 
 * This file is part of DILLOXL.
 *
 * DILLOXL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DILLOXL is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DILLOXL. If not, see <http://www.gnu.org/licenses/>.
 * 
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

    bool build();
    void run();

  private:
    class Impl; Impl* m_pImpl;
  };
}

#endif // dilloxl_user_program_h
