/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */
#include "dilloatello_dillo_linguaggio_didattico.h"

programma_inizio
  crea_oggetto_da_astrazione(TelloDrone) dando_nome(topolino)
  messaggia_oggetto(topolino, connect, ())
  messaggia_oggetto(topolino, video_enable, (true))
  messaggia_oggetto(topolino, takeoff, ())
  crea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(contatore_di_volte)
  crea_oggetto_da_astrazione( numero_con_la_virgola) dando_nome(altezza)
  crea_oggetto_da_astrazione( numero_con_la_virgola) dando_nome(distanza_y_percorsa)
  contatore_di_volte = 0;
  distanza_y_percorsa = 0;
  altezza = chiedi_informazione_a(topolino) cosa(height_from_floor)
  fai_questo
    se_e_vero_che(altezza < 500) allora_fai_questo
      messaggia_oggetto(topolino, go_U, (100))
    fine_questo
    messaggia_oggetto(topolino,      forward, (100))
    contatore_di_volte = contatore_di_volte + 1;
    distanza_y_percorsa += 100;
    altezza = chiedi_informazione_a(topolino) cosa(height_from_floor)
  e_rifallo_finche_e_vero_che(
            (contatore_di_volte < 10) 
    e_anche (altezza > 200)
    e_anche (distanza_y_percorsa <= 1000))
  messaggia_oggetto(topolino, land, ())
programma_fine
