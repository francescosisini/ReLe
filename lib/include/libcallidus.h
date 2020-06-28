/* ML */
char * callidus_GET(int cod_servizio);

/* Entità */

typedef enum
  {
    INDIPENDENTE,
    CENTRALE
  }Riscaldamento;

typedef struct {
  /* chiave */
  int id;
  /* campi */
  int area; //m^2
  int piani;
  int bagni;
  int giardino; //m^2
  int distanza_centro;//m
  int vani;
  Riscaldamento tipo_r;
  int lati_indipendenti;
}Immobile;

/* Gestione dati*/

int db_inserisci_immobile(Immobile *im);

int  db_seleziona_immobile(Immobile *t, Immobile clausola,int da_chiave);

void db_inizializza_immobile(Immobile *im);

int db_aggiorna_immobile(Immobile c);
