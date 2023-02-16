/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */

#include <cstdarg>
#include <cstdio>
#include "sistema.h"

static const size_t gszStatusY  = 1;
static       size_t gszCurLineY = 2;
static char thread_local gBuf[4096];

void stampa(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  vsnprintf(gBuf, sizeof(gBuf), fmt, arglist);
  va_end(arglist);

  std::fprintf(stderr, "\033[%zu;%dH\033[K", gszCurLineY, 1);
  std::fprintf(stderr, "%s", gBuf);
  gszCurLineY += 1;
  if (gszCurLineY > 10) { gszCurLineY = 1; }
}

void stampa_stato(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  vsnprintf(gBuf, sizeof(gBuf), fmt, arglist);
  va_end(arglist);

  std::fprintf(stderr, "\033[%zu;%dH\033[K", gszStatusY, 1);
  std::fprintf(stderr, "%s", gBuf);
}

void stampa_errore(const char* fmt, ...)
{
  va_list arglist;
  va_start(arglist, fmt);
  vsnprintf(gBuf, sizeof(gBuf), fmt, arglist);
  va_end(arglist);

  std::fprintf(stderr, "\033[%zu;%dH\033[K", gszStatusY, 1);
  std::fprintf(stderr, "[ERRORE] %s", gBuf);
}

void cancella_terminale()
{
  std::fprintf(stderr, "\033[2J");
}