/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */
#include "dillo_linguaggio_didattico.h"

programma_inizio
  crea_oggetto_da_astrazione(TelloDrone) dando_nome(topolino)
  messaggia_oggetto(topolino, connect, ())
  messaggia_oggetto(topolino, video_enable, (true))
  messaggia_oggetto(topolino, takeoff, ())
  crea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(contatore_di_volte)
  crea_oggetto_da_astrazione(numero_con_la_virgola) dando_nome(altezza)
  contatore_di_volte = 0;
  altezza = chiedi_informazione_a(topolino) cosa(height_from_floor)
  fai_questo_finche_e_vero_che((contatore_di_volte < 10) e_anche (altezza > 200))
    messaggia_oggetto(topolino,         go_U, (100))
    messaggia_oggetto(topolino,         go_R, (300))
    messaggia_oggetto(topolino,      forward, (300))
    messaggia_oggetto(topolino,       rot_CC, ( 90))
    messaggia_oggetto(topolino,      forward, (300))
    messaggia_oggetto(topolino,       rot_CC, ( 90))
    messaggia_oggetto(topolino,      forward, (300))
    messaggia_oggetto(topolino,       rot_CC, ( 90))
    messaggia_oggetto(topolino,         go_D, (100))
    contatore_di_volte = contatore_di_volte + 1;
    altezza = chiedi_informazione_a(topolino) cosa(height_from_floor)
  fine_questo
  messaggia_oggetto(topolino, flip, ("l"))
  messaggia_oggetto(topolino, land, ())
programma_fine


/*
./DilloATello | ffplay -fflags nobuffer -fflags discardcorrupt -flags low_delay -framedrop -avioflags direct -
*/
