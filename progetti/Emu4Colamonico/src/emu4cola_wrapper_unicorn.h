/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-01-30
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#ifndef emu4cola_wrapper_unicorn_h
#define emu4cola_wrapper_unicorn_h

#include "emu4cola_incs.h"
#include <unicorn/unicorn.h>

namespace cola
{
  class UnicornWrapper {
  public:
    UnicornWrapper();
    UnicornWrapper(const UnicornWrapper&) = delete;
    UnicornWrapper(UnicornWrapper&&) noexcept;
    UnicornWrapper& operator=(const UnicornWrapper&) = delete;
    UnicornWrapper& operator=(UnicornWrapper&&) noexcept;
    virtual ~UnicornWrapper();



  private:
    class Impl;
    Impl* m_pImpl;
  };
}

#endif // emu4cola_wrapper_unicorn_h
