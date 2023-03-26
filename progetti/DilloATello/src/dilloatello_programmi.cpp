/* ==========================================================================
 * TELLO, Progetto Didattico per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20221215
 * ========================================================================== */
#include "dilloatello_dillo_linguaggio_didattico.h"

class Ombrello { public: void apri() {}; void chiudi() {}; };
class PioggiaDetector { public: bool sta_piovendo() const { return true; } };

/* ==========================================================================
 * DILLO, Didattica per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20230326
 * ========================================================================== */
programma_inizio
  crea_oggetto_da_astrazione(Ombrello) dando_nome(il_mio_ombrello)
  crea_oggetto_da_astrazione(PioggiaDetector) dando_nome(il_mio_detector)
  crea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(contatore_di_passi)
  crea_oggetto_da_astrazione(bandierina_vero_falso) dando_nome(piove)
  contatore_di_passi = 0;
  piove = falso;
  fai_questo
    piove = chiedi_a(il_mio_detector, sta_piovendo, ())
    se_e_vero_che(piove) allora_fai_questo
      messaggia_oggetto(il_mio_ombrello, apri, ())
    altrimenti_fai_questo
      messaggia_oggetto(il_mio_ombrello, apri, ())
    fine_questo
    contatore_di_passi = contatore_di_passi + 1;
  e_rifallo_finche_e_vero_che(contatore_di_passi < 10)
programma_fine

/* ==========================================================================
 * DILLO, Didattica per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20230326
 * ========================================================================== */
programma_inizio
  crea_oggetto_da_astrazione(Ombrello) dando_nome(il_mio_ombrello)
  crea_oggetto_da_astrazione(PioggiaDetector) dando_nome(il_mio_detector)
  crea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(contatore_di_passi)
  crea_oggetto_da_astrazione(bandierina_vero_falso) dando_nome(piove)
  contatore_di_passi = 0;
  piove = falso;
  fai_questo_per_volte(10)
    piove = chiedi_a(il_mio_detector, sta_piovendo, ())
    se_e_vero_che(piove) allora_fai_questo
      messaggia_oggetto(il_mio_ombrello, apri, ())
    altrimenti_fai_questo
      messaggia_oggetto(il_mio_ombrello, apri, ())
    fine_questo
    contatore_di_passi = contatore_di_passi + 1;
  fine_questo
programma_fine

/* ==========================================================================
 * DILLO, Didattica per Colamonico
 * Autore: Michele Iacobellis
 *   Data: 20230326
 * ========================================================================== */
programma_inizio
  crea_oggetto_da_astrazione(Ombrello) dando_nome(il_mio_ombrello)
  crea_oggetto_da_astrazione(PioggiaDetector) dando_nome(il_mio_detector)
  crea_oggetto_da_astrazione(numero_intero_positivo) dando_nome(contatore_di_passi)
  crea_oggetto_da_astrazione(bandierina_vero_falso) dando_nome(piove)
  contatore_di_passi = 0;
  piove = falso;
  fai_questo_finche_e_vero_che(contatore_di_passi < 10)
    piove = chiedi_a(il_mio_detector, sta_piovendo, ())
    se_e_vero_che(piove) allora_fai_questo
      messaggia_oggetto(il_mio_ombrello, apri, ())
    altrimenti_fai_questo
      messaggia_oggetto(il_mio_ombrello, apri, ())
    fine_questo
    contatore_di_passi = contatore_di_passi + 1;
  fine_questo
programma_fine


