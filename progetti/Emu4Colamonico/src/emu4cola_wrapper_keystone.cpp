/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * EMULATORE PER COLAMONICO
 *   AUTORE: Michele Iacobellis
 *     DATA: 2023-01-30
 * VERSIONE: 1.0
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */ 

#include "emu4cola_wrapper_keystone.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
class cola::KeystoneWrapper::Impl {
public:
   Impl();
  ~Impl();
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper::KeystoneWrapper()
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper::KeystoneWrapper(KeystoneWrapper&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper& 
  cola::KeystoneWrapper::operator=(KeystoneWrapper&& oth) noexcept
{
  delete m_pImpl;
      m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper::~KeystoneWrapper()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper::Result 
  cola::KeystoneWrapper::compile(const std::string& src
    , const std::string& opt)
{
  ks_engine* ks;
  ks_err err;
  size_t count;
  uint8_t* encode;
  size_t size;

  err = ks_open(KS_ARCH_X86, KS_MODE_32, &ks);
  if (err != KS_ERR_OK) {
    printf("ERROR: failed on ks_open(), quit\n");
    return std::make_tuple(false, std::vector<uint8_t>());
  }

  if (ks_asm(ks, src.c_str(), 0, &encode, &size, &count) != KS_ERR_OK) {
    printf("ERROR: ks_asm() failed & count = %zu, error = %u (%s)\n",
      count, ks_errno(ks), ks_strerror(ks_errno(ks)));
  }
  else {
    size_t i;

    printf("%s = ", src.c_str());
    for (i = 0; i < size; i++) {
      printf("%02x ", encode[i]);
    }
    printf("\n");
    printf("Compiled: %zu bytes, statements: %zu\n", size, count);
  }

  ks_free(encode);
  ks_close(ks);

  std::vector<uint8_t> vout;
  for (size_t k = 0; k < size; ++k) {
    vout.push_back(*encode++);
  }

  return std::make_tuple(true, vout);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper::Impl::Impl()
{
  
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
cola::KeystoneWrapper::Impl::~Impl()
{

}

