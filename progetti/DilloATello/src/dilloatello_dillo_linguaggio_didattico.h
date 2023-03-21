/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */

#ifndef dillo_linguaggio_didattico_h
#define dillo_linguaggio_didattico_h

#include "dilloatello_tello.h"
#include "dilloatello_sistema.h"

#define programma_inizio \
  int programma_tello() { \
    cancella_terminale();\
    stampa("IL PROGRAMMA È INIZIATO...\n");

#define programma_fine \
  stampa("IL PROGRAMMA È FINITO.\n"); \
  return 0; }

#define numero_intero_positivo size_t
#define numero_con_la_virgola float

#define crea_oggetto_da_astrazione(tipo) tipo
#define dando_nome(nome) nome;
#define messaggia_oggetto(nome,msg,arg) \
  stampa("METTO IN CODA IL MESSAGGIO " #msg " A " #nome "\n"); \
  nome.msg arg;

#define fine_questo }
#define per_sempre_fai_questo \
  stampa("STO PER FARE PER SEMPRE QUELLO CHE SCRIVI DOPO:\n"); \
  for(;;){
#define fai_questo_per_volte(a) \
  stampa("STO PER FARE " #a " VOLTE QUELLO CHE SCRIVI DOPO:\n"); \
  for (size_t passo_corrente = 0; passo_corrente < a; ++passo_corrente) {
#define fai_questo_finche_e_vero_che(a) \
  stampa("STO PER FARE FINCHE' E' VERO CHE " #a " QUELLO CHE SCRIVI DOPO:\n"); \
  while (a) {
#define fai_questo \
  stampa("STO PER FARE QUELLO CHE SCRIVI DOPO:\n"); \
  do {
#define e_rifallo_finche_e_vero_che(a) \
  stampa("E LO RIFACCIO SE E' VERO CHE " #a "...\n"); \
  } while (a);
#define se_e_vero_che(a)      if (a)
#define allora_fai_questo     {
#define altrimenti_fai_questo else {

#define fai_dormire_il_processo_per_millisecondi(x) \
  stampa("IL PROCESSO DORME PER %u ms...\n",x); \
  std::this_thread::sleep_for(std::chrono::milliseconds(x));

#define stampa_sulla_console_lo_stato_di(a)\
  do {\
    const auto& st = a.lastStatus();\
    stampa_stato("STATO DI " #a ": P,R,Y = %f,%f,%f ALTEZZA = %f\n"\
      , st.u.status.pitch\
      , st.u.status.roll\
      , st.u.status.yaw\
      , st.u.status.height_from_floor);\
  } while(0);

#define chiedi_informazione_a(a)\
  a.lastStatus().u.status
#define cosa(a) .a;
#define e_anche &&

#endif // dillo_linguaggio_didattico_h
