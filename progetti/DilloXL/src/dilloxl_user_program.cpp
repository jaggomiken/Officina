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
#include "dilloxl_user_program.h"
#include "dilloxl_if.h"
#include <atomic>
#include <thread>

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 * INCLUDES FOR OPERATING SYSTEM
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< */
#ifdef WIN32
# include <Windows.h>
# include <userenv.h>
#else
# include <cstdlib>
# include <unistd.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <cstring>
# include <cerrno>
# include <dlfcn.h>
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
  bool build();
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
    "// COMANDI DEL DRONE:\n"
    "// \t-vai_a_sinistra_cm\n"
    "// \t-vai_a_destra_cm\n"
    "// \t-vai_su_cm\n"
    "// \t-vai_giu_cm\n"
    "// \t-ruota_senso_orario_gradi\n"
    "// \t-ruota_senso_antiorario_gradi\n"
    "// \t-vai_avanti_cm\n"
    "// \t-vai_indietro_cm\n"
    "// \t-decolla\n"
    "// \t-atterra\n"
    "// \t-stop\n"
    "\n"
    "programma_inizio\n"
	  "\tcrea_oggetto_da_astrazione(Drone) dando_nome(plinio)\n"
    "\tcrea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(passi)\n"
    "\tpassi = 0;\n"
    "\tmessaggia_oggetto( plinio, video, (falso) )\n"
	  "\tmessaggia_oggetto( plinio, decolla, (0) )\n"
    "\tfai_questo_finche_e_vero_che(passi < 3)\n"
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
bool dilloxl::UserProgram::build()
{
  DILLOXL_CAPTURE_CPU(nullptr == m_pImpl, "Puntatore a Impl è NULL");
  return m_pImpl->build();
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
bool dilloxl::UserProgram::Impl::build()
{
  FILE* fp = ::fopen(DILLOXL_IF_SOURCEFILENAME, "w+");
  if (nullptr != fp) {
    ::fprintf(fp, "%s", m_strSource.c_str());
    ::fclose (fp);
  } else {
    std::fprintf(stderr, "[PRO]: Impossibile salvare il file.\n");
    return false;
  }

#ifdef WIN32
  static char pcmd[8192];
  const auto* p = TEXT("\"c:\\msys64\\ucrt64\\bin\\g++\" "
    "-shared " DILLOXL_IF_SOURCEFILENAME " "
    "-o " DILLOXL_IF_DLLNAME " "
    "-I./bin -I../include" " "
    "-L./bin -L../lib libdilloxl.dll.a");
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
      , "[PRO]: Impossibile lanciare il compilatore (%d)\n"
      , ::GetLastError());
  } else {
    DWORD wExit = DWORD(-1);
    std::fprintf(stderr, "[PRO]: Controllo programma...\n");
    WaitForSingleObject( pi.hProcess, INFINITE );
    GetExitCodeProcess(pi.hProcess, &wExit);
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );    
    std::fprintf(stderr, "[PRO]: Controllato (Esito %u).\n", wExit);
    return (0 == wExit);
  }
  return false;
#elif defined(__APPLE__)
  auto pid = fork();
  if (0 == pid) {
    const char* pprg   = "/usr/bin/cc";
    const char* argv[] = {
        "/usr/bin/cc"
      , "-bundle"
      , "-bundle_loader"
      , "./dilloxl-exec"
      , "-o"
      , DILLOXL_IF_DLLNAME
      , DILLOXL_IF_SOURCEFILENAME
      , NULL
    };
    std::fprintf(stderr, "[PRO]: Controllo programma...\n");
    if (::execve(pprg, const_cast<char*const*>(argv), nullptr) < 0) {
      std::fprintf(stderr
        , "[PRO]: Errore nel lanciare il compilatore (%s)\n"
        , ::strerror(errno));
    }
  } else {
    int32_t st = -1;
    if (0 > ::waitpid(pid, &st, 0)) {
      std::fprintf(stderr
        , "[PRO]: Errore nell'attendere esito compilatore (%s)\n"
        , ::strerror(errno));
    } else {
      std::fprintf(stderr, "[PRO]: Controllato (Esito %d).\n", st);
      return (0 == st);
    }
  }
  return false;
#else
  return false;
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
      , "[PRO]: Errore: LoadLibrary, %d\n", ::GetLastError());
  } else {
    auto fn = GetProcAddress(hm, DILLOXL_IF_SYMBOL);
    if (NULL == fn) {
      std::fprintf(stderr
        , "[PRO]: Errore: GetProcAddress(%s), %d\n"
        , DILLOXL_IF_SYMBOL, ::GetLastError());
    }
    else {
      SetupUserProgramContext(&m_upc, this);
      std::fprintf(stderr
        , "[PRO]: Sto per lanciare il programma utente...\n");
      typedef void (*UP)(UserProgramContext*);
      UP fn_up = UP(fn); fn_up(&m_upc);
      std::fprintf(stderr
        , "[PRO]: Fatto. Che si vede?\n");
    }
    ::FreeLibrary(hm);
  }
#else
  auto hm = ::dlopen(DILLOXL_IF_DLLNAME, RTLD_LOCAL);
  if (NULL == hm) {
    std::fprintf(stderr
      , "[PRO]: Errore: dlopen, %s\n", ::dlerror());
  } else {
    auto fn = ::dlsym(hm, DILLOXL_IF_SYMBOL);
    if (NULL == fn) {
      std::fprintf(stderr
        , "[PRO]: Errore: dlsym(%s), %s\n"
        , DILLOXL_IF_SYMBOL, ::dlerror());
    }
    else {
      SetupUserProgramContext(&m_upc, this);
      std::fprintf(stderr
        , "[PRO]: Sto per lanciare il programma utente...\n");
      typedef void (*UP)(UserProgramContext*);
      UP fn_up = UP(fn); fn_up(&m_upc);
      std::fprintf(stderr
        , "[PRO]: Fatto. Che si vede?\n");
    }
    ::dlclose(hm);
  }
#endif
}
