/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * DILLO EXTRA LARGE - DILLOXL
 * (C) Copyright by Michele Iacobellis
 * A project for students...
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#include "dilloxl_user_program.h"
#include "dilloxl_if.h"
#include <atomic>
#include <thread>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * INCLUDES FOR OPERATING SYSTEM
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#define WIN32
#ifdef WIN32
# include <Windows.h>
# include <userenv.h>
#else
#endif

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * LOCALS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * CLASS
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
class dilloxl::UserProgram::Impl {
public:
  Impl();
 ~Impl();
  void build();
  void run();

  std::string m_strName;
  std::string m_strSource;
  UserProgramContext m_upc;
};

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::UserProgram::Configure(int32_t argc, char* argv[])
{
#ifdef WIN32  
  ::SetEnvironmentVariableA("PATH", "c:\\msys64\\ucrt64\\bin;%PATH%");
#else
#endif  
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
static dilloxl::UserProgram* gpCur = nullptr;
dilloxl::UserProgram& dilloxl::UserProgram::GetCurrent()
{
  DILLOXL_CAPTURE_CPU(nullptr == gpCur, "Programma corrente non disponibile");
  return *gpCur;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::UserProgram::UserProgram(const std::string& name)
  : m_pImpl{ nullptr }
{
  m_pImpl = new(std::nothrow) Impl{};
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Errore allocazione");
  m_pImpl->m_strName   = name;
  m_pImpl->m_strSource =
    "#include \"dilloxl\"\n\n"
    "//\n"
    "// Il mio primo programma che fa volare il drone Tello.\n"
    "// Il tuo nome e cognome: <scrivi qui>\n"
    "// La data di oggi: AAAA-MM-GG\n"
    "//\n"
    "programma_inizio\n"
	  "\tcrea_oggetto_da_astrazione(Drone) dando_nome(plinio(TELLO))\n"
    "\tcrea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(passi)\n"
	  "\tmessaggia_oggetto( plinio, decolla, (0) )\n"
    "\tfai_questo_finche_e_vero_che(passi < 10)\n"
    "\t\tmessaggia_oggetto( plinio, vai_avanti_cm, (100))\n"
    "\t\tpassi = passi + 1;\n"
    "\tfine_questo\n"
	  "\tmessaggia_oggetto( plinio, atterra, (0) )\n"
    "programma_fine\n"
  ;
  gpCur = this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::UserProgram::UserProgram(UserProgram&& oth) noexcept
  : m_pImpl{ nullptr }
{
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::UserProgram& 
    dilloxl::UserProgram::operator=(UserProgram&& oth) noexcept
{
  delete m_pImpl;
  m_pImpl = oth.m_pImpl;
  oth.m_pImpl = nullptr;
  return *this;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::UserProgram::~UserProgram()
{
  delete m_pImpl;
  m_pImpl = nullptr;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::UserProgram::name() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_strName;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::UserProgram::setSource(const std::string& value)
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->m_strSource = value;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
std::string dilloxl::UserProgram::source() const
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->m_strSource;
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::UserProgram::build()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->build();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::UserProgram::run()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  m_pImpl->run();
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::UserProgram::Impl::Impl()
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
dilloxl::UserProgram::Impl::~Impl()
{
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::UserProgram::Impl::build()
{
  FILE* fp = ::fopen(DILLOXL_IF_SOURCEFILENAME, "w+");
  if (nullptr != fp) {
    ::fprintf(fp, "%s", m_strSource.c_str());
    ::fclose (fp);
  } else {
    std::fprintf(stderr, "[UP]: Impossibile salvare il file.\n");
    return;
  }

#ifdef WIN32
  static char pcmd[8192];
  const auto* p = TEXT("\"c:\\msys64\\ucrt64\\bin\\g++\" "
    "-shared " DILLOXL_IF_SOURCEFILENAME " "
    "-o " DILLOXL_IF_DLLNAME " "
    "-I./bin -I../include" " "
    "-L./bin -L../lib");
  memcpy(pcmd, p, std::min(sizeof(pcmd), ::strlen(p)));
  STARTUPINFOA si;
  PROCESS_INFORMATION pi;
  ZeroMemory( &si, sizeof(si) ); si.cb = sizeof(si);
  ZeroMemory( &pi, sizeof(pi) );
  auto res = ::CreateProcessA(
      NULL
    , pcmd
    , NULL
    , NULL
    , TRUE
    , 0
    , NULL
    , NULL
    , &si
    , &pi
  );
  if (0 == res) {
    std::fprintf(stderr
      , "[UP]: Impossibile lanciare il compilatore (%d)\n"
      , ::GetLastError());
  } else {
    std::fprintf(stderr, "[UP]: Controllo programma...\n");
    WaitForSingleObject( pi.hProcess, INFINITE );
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );    
    std::fprintf(stderr, "[UP]: Controllato.\n");
  }
#else
#endif
}

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * METHOD
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
void dilloxl::UserProgram::Impl::run()
{
#ifdef WIN32
  auto hm = ::LoadLibraryA(DILLOXL_IF_DLLNAME);
  if (NULL == hm) {
    std::fprintf(stderr
      , "[UP]: Errore: LoadLibrary, %d\n", ::GetLastError());
  } else {
    auto fn = GetProcAddress(hm, DILLOXL_IF_SYMBOL);
    if (NULL == fn) {
      std::fprintf(stderr
        , "[UP]: Errore: GetProcAddress(%s), %d\n"
        , DILLOXL_IF_SYMBOL, ::GetLastError());
    }
    else {
      SetupUserProgramContext(&m_upc, this);
      std::fprintf(stderr
        , "[UP]: Sto per lanciare il programma utente...\n");
      typedef void (*UP)(UserProgramContext*);
      UP fn_up = UP(fn); fn_up(&m_upc);
      std::fprintf(stderr
        , "[UP]: Fatto. Che si vede?\n");
    }
    ::FreeLibrary(hm);
  }
#else
#endif
}
