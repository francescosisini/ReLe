/*______________________________________________
 | ReLe1.0
 | 
 | Francesco e Valentina Sisini (c) 2020
 */
#include <stdio.h>

typedef struct
{
  /** Interfacciamento */
  
  double * strato_ingresso;
  double * strato_uscita;
  
  /** Topologia rete */
  
  /* Distribuzione dei neuroni negli strati della rete */ 
  int N_neuroni_sensitivi;
  int N_neuroni_afferenti;
  int N_neuroni_primo_strato_intercalare; 
  int N_neuroni_secondo_strato_intercalare;
  int N_strati_computazionali;

  /* neuroni e dendriti*/
  int l1_np, l1_nd;
  int l2_np, l2_nd;
  int l3_np, l3_nd;

  
  /** Qualità della rete */
  
  /* Errore quadratico */
  double EQ;

  /* Iterazioni di addestramento */
  int iterazioni;

  /* Errore quadratico medio = EQ/iterazioni*/
  double EQM;


  /** Rappresentazione in memoria */

  /* Risultato atteso, o taret output */
  double * v_d;

  /* Strato 1: dall'ingresso 0 all'uscita 1 */
  double * v_x0;
  double * v_t;
  double * v_s1;
  double * v_y1;

  /* Strato 2: dall'ingresso 1 all'uscita 2 */
  double * v_x1;
  double * v_u;
  double * v_u_tmp;
  double * v_s2;
  double * v_y2;
  double * v_dy2;

  /* Strato 3: dall'ingresso 2 all'uscita 3 */
  double * v_x2;
  double * v_v;
  double * v_v_tmp;
  double * v_s3;
  double * v_y3;
  double * v_dy3;
} rele_rete;


typedef struct
{
  double fattore_apprendimento;
  double max_per_normalizzazione;
  int seme_pseudocasuale;
  int epoche;
  int campioni;
}rele_parametri;


/* crea una rete neurale che può essere addestrata.
   Inizializza i pesi dendritici (pesi delle connessioni)
   tra -0.1 e 0.1
 */
rele_rete * rele_Crea_rete(
			   int N_neuroni_sensitivi,
			   int N_neuroni_afferenti,
			   int N_neuroni_primo_strato_intercalare,
			   int N_neuroni_secondo_strato_intercalare);

/* libera le risorse della rete */
void  rele_Libera_rete(rele_rete * rete);

/* 
   addestra una rete neurale usando i dati e le classi passate
   ogni chiamata viene eseguita una singola iterazione
*/
rele_rete *  rele_Addestra(rele_rete * rn,
			   rele_parametri * par,
			   double * dati,
			   double * classi);
/* 
   Usa una rete già addestrata per classificare i dati 
   il risultato è puntato da rete->strato_uscita
*/
void  rele_Classifica(rele_rete * rete, double * dati);


void rele_Salva(rele_rete * rete, FILE * f);

rele_rete * rele_Apri(FILE * f);


/*___________________________________________________________________________________ 
 *|
 *| v_s: vettore delle somme dei canali dendritici per gli n_perc percettroni
 *| v_y: vettore degli output per gli n_perc percetroni
 *| v_w: vettore dei pesi dendritici per gli n_perc percetroni
 *| v_x: vettore degli input al percettrone (uguale per tutti  gli n_perc percetroni)
 *| n_perc: numero di percettroni nello strato
 *| n_dend: numero di dendriti per percettrone */
void layer_feed_forward(double v_s[],double v_y[],double v_w[],double v_x[],int n_perc, int n_dend);

/*
  v_w vettore di dimensione n_dend+1. Il primo elemento è 1, il resto sono i pesi sinaptici
  v_x vettore dell'input del percettrone. Il primo elemento è 1
  z somma pesata dell'input: v_w < . v_x
  d Valore Atteso - Valore Calcolato
  rate learning rate
  n_dend numero dei dendriti del percettrone
 */
void perc_correzione(double v_w[],double v_x[],double z,double d,double rate,int n_dend);

/* Mappa l'output v_y in v_x aggiungendo prima l'elemento v_x[0]=1*/
void layer_map_out_in(double v_x[],double v_y[], int n_dend);

/*
  v_w: vettore di dimensione  n_dend+1 di pesi dendritici
  v_x: vettore degli n_dend+1 (c'è il bias) input al percettrone
  n_dend numero di dendriti  
*/
double perc_calc_output(double v_w[],double v_x[],int n_dend);

/* Corregge i pesi del percettrone */
void perc_correzione(double v_w[],double v_x[],double z,double d,double rate,int n_dend);

/* Calcola il valore della risposta del percettrone*/
double activ_function(double summed_input);

/* Calcola il valore della derivata della risposta del percettrone*/
double Dactiv_function(double summed_input);

/* Calcola e aggiorna l'EQM in base all'ultimo EQ e il numeto di iterazioni eseguite */
void aggiorna_EQM(rele_rete * rn);


/*** AG ***/

typedef double allele;

/*
 * Contiene le informazioni sulla struttura dei cromosomi
 */
typedef struct
{
  /* Numero totale di geni*/
  int N_geni;
  /* NUmero di geni nei vari strati. La somma deve corrispondere a N_geni */
  int n_geni_l1;
  int n_geni_l2;
  int n_geni_l3;
  

  double idoneita;
  
  /*Il buffer gene viene creato di dimensione N_geni dalla rele_AG_Crea_cromosomi() */
  allele * gene;
  
} rele_croma;

/*
 * Rappresenta un batch di addestramento
 * costituito da 'numero' di esempli completi
 * di dati_input e dati_output che rappresentano le classi attese
 */
typedef struct
{
  int numero;
  double * dati;
  double * classi;
} rele_batch;

/*  
 * Alloca e torna un array di n rele_croma (quindi una popolazione di cromosomi)
 * Valorizza il parametro N_Geni ottenendolo dal modello
 */
rele_croma * rele_AG_Crea_cromosomi(rele_rete * modello, int n);
void rele_AG_Libera_cromosomi(rele_croma * c, int n);

/*
 * Calcola l'ideoneità di 'cromosoma' 
 * e la memorizza nel campo ideoneità del cromosoma stesso
 */
void rele_AG_Calcola_idoneita_cromosoma(rele_croma * cromosoma,
			   rele_rete * modello,
			   rele_batch b);

/*
 * Calcola uno score random (con funzione generatrice piatta) per 
 * ognuni degli n cromosomi.
 * Restituisce l'indice del cromosoma che risulta avere 
 * il prodotto score*idoneità più elevatoF
 * Esclude dalla selezione l'elemento di indice 'escluso'
 * Moltiplica l'idoneità del cromosoma per un random compreso 1 e 1-(soglia_di_score),
 * per lasciare immutata l'idoneità impostare il parametro a 0
 */
int rele_AG_selezione(rele_croma * cromosomi, int n,int escluso,double soglia_score);

/*
 * Valorizza i geni di figlio_1 e _2 con il risultato
 * dell'incrcio dei cromosomi genitore 1 e 2
 */
void rele_AG_incrocia(rele_croma genitore_1,
		      rele_croma genitore_2,
		      rele_croma * figlio_1,
		      rele_croma * figlio_2);

/*
 * Muta ogni gene della cromosoma con probabilità p fino al massimo di +-x
 */
void rele_AG_muta(rele_croma * cromosoma, double p, double x);

/*
 * Valorizza i pesi delle sinapsi della rete r con i valor dei geni 
 * del cromosoma 
 */
void rele_AG_trascrivi_sinapsi(rele_croma  cromosoma, rele_rete * r);


/*
 * Stampa sul terminale l'istogramma delle idoneità della popolazione di cromosomi
 * popolazione: array di rele_croma
 * n: numero cromosomi nella popolazione
 * label: etichetta per l'istogramma
 * indice: indice per la collocazione di più diagrammi nello schermo
 * numero_bin: il numero di bin dell'istrogramma (tipico 15)
 * altazza_massima: l'altezza dell'istogramma in corrispondenza di un bin popolato con
 * l'intera popolazione
 */
void rele_AG_stampa_popolazione(rele_croma * popolazione, int n, char * label,int indice);





