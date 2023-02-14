/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-01-30
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 

#ifndef emu4cola_wrapper_keystone_h
#define emu4cola_wrapper_keystone_h

#include "emu4cola_incs.h"
#include <keystone/keystone.h>

namespace cola
{
  class KeystoneWrapper {
  public:
    using Result = std::tuple<bool, std::vector<uint8_t>>;

    KeystoneWrapper();
    KeystoneWrapper(const KeystoneWrapper&) = delete;
    KeystoneWrapper(KeystoneWrapper&&) noexcept;
    KeystoneWrapper& operator=(const KeystoneWrapper&) = delete;
    KeystoneWrapper& operator=(KeystoneWrapper&&) noexcept;
    virtual ~KeystoneWrapper();
    virtual Result
      compile(const std::string& src, const std::string& opt = {});

  private:
    class Impl;
    Impl* m_pImpl;
  };
}

#endif // emu4cola_wrapper_keystone_h
