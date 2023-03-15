/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */

#include <mutex>
#include <queue>
#include <cstdarg>
#include <cstdio>
#include "dilloatello_sistema.h"

static const size_t gszStatusY  = 1;
static       size_t gszCurLineY = 2;
static char thread_local gBuf[4096];
static std::queue<std::string> gQueueMsgs;
static std::mutex gMtxQueueMsgs;

void stampa(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  vsnprintf(gBuf, sizeof(gBuf), fmt, arglist);
  va_end(arglist);
  
  std::lock_guard<std::mutex> g{ gMtxQueueMsgs };
  gQueueMsgs.push(gBuf);
}

void stampa_stato(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  vsnprintf(gBuf, sizeof(gBuf), fmt, arglist);
  va_end(arglist);

  std::lock_guard<std::mutex> g{ gMtxQueueMsgs };
  gQueueMsgs.push(gBuf);
}

void stampa_errore(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  vsnprintf(gBuf, sizeof(gBuf), fmt, arglist);
  va_end(arglist);

  std::lock_guard<std::mutex> g{ gMtxQueueMsgs };
  gQueueMsgs.push("[ERRORE] " + std::string(gBuf));
}

void cancella_terminale()
{
  
}

std::string scoda_prossimo_messaggio()
{
  std::lock_guard<std::mutex> g{ gMtxQueueMsgs };
  if (!gQueueMsgs.empty()) {
    auto ret = gQueueMsgs.front();
    gQueueMsgs.pop();
    return ret;
  }
  return{};
}
