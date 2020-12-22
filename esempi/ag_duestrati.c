#include "librele.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define ITERAZIONI 50000
#define BATCH_SIZE 550
#define NEUR_SENS 2
#define NEUR_AFF 1
#define N_CROMOSOMI 100
#define EQM_ACCETTABILE 1./25.
/*
  COMPILAZIONE
  gcc -o ag_uno_strato ag_uno_strato.c -I../lib/include ../lib/librele.o -lm
 */


/*
 * In questo esempio una popolazione di cromosomi viene usata per identificare
 * i valori delle conessioni sinpatiche di una rete neurale che deve distinguere
 * i puti che stanno sotto la parabola di equazione y=-x^2+4.
 * I dati di training vengono generati contrstualmente all'esecuzione dell'
 * esempio usando una funzione generatrice 
 * Durante l'esecuzione dell'esempio viene mostrato l'istogramma della distribizione
 * dei cromosomi rispetto al livello di idoneità
 * L'addestramento termina quando l'errore quadratico medio della rete è ritenuto accettabile
 * Al termine dell'addestramento la stessa funzione generatrice viene usata per tesatre 
 * la rete usando dei nuovi dati pseudo-casuali
 * Le terne (x,y,classe) sono stampate a video:
 * la coppia (x,Y) è generata dalla funzione generatrice e la classe  è calcolata dalle rete 
 * e possono essere rappresentate graficamente salvando l'output su un file csv
 * La configurazione dei pesi sinaptici della rete è in fine salvata su sul file "rete.csv"
*/


int main()
{
  
  printf("\x1b[2J"); // Cancello schermo
  rele_rete * r =  rele_Crea_rete(NEUR_SENS,NEUR_AFF,30,0);

  /* crea due popolazioni di n_c cromosomi: genitori e figli */
  int n_c = N_CROMOSOMI;
  int n_u = n_c/2; //N_c deve essere pari
  rele_croma * pop_g = rele_AG_Crea_cromosomi(r, n_c);
  rele_croma * pop_f = rele_AG_Crea_cromosomi(r, n_c);

  /* dati addestramento */
  double d[NEUR_SENS*BATCH_SIZE];
  double c[NEUR_AFF*BATCH_SIZE];
  double m = 1.;

  rele_batch b;
  b.numero = BATCH_SIZE;

  srand(time(0));
  int casi = 0;
  int classe_precedente = 0;
  for(int i=0;i<ITERAZIONI;i++)
    {
      do
	{
	  /* Genera le cooridnate di un punto nel piano */
	  d[0+NEUR_SENS*casi]=-2+(double)rand()/(double)RAND_MAX*4.;
	  d[1+NEUR_SENS*casi]=(double)rand()/(double)RAND_MAX*5;
	  
	  /* il punto è classificato come 1 se giace sotto la parabola */
	  if(-d[0+NEUR_SENS*casi]*d[0+NEUR_SENS*casi]+4<d[1+NEUR_SENS*casi])
	    {
	      c[0+NEUR_AFF*casi]=0;
	    }
	  else
	    {
	      c[0+NEUR_AFF*casi]=1;
	    }
	  if(c[0+NEUR_AFF*casi] == classe_precedente)
	    continue;
	  classe_precedente = c[0+NEUR_AFF*casi];
	  casi++;
	  
	}
      while(casi<(b.numero-1));
      /*dati e classi pronti */
      casi = 0;
      /* batch popolato di dati */
      b.dati = d;
      b.classi = c;
      
      /* Calcola le idoenità relative al batch */
      double i_media = 0;
      for(int k=0; k<n_c; k++)
	{
	  rele_AG_Calcola_idoneita_cromosoma(pop_g+k,r,b);
	  
	  i_media += (pop_g+k)->idoneita;
	}
      i_media=i_media/(double)n_c;
      rele_AG_stampa_popolazione(pop_g, n_c, "Parabola",1);
      
      if(i_media>1./(EQM_ACCETTABILE))
	{
	  /* la rete è addestrata a sufficienza */
	  break;
	}

      /* Seleziona i cromosomi */
      double p_muta = 0.99;
      	for(int k=0;k<n_u;k++)
	  {
	    /* Genera due figli */
	    int g_1 = rele_AG_selezione(pop_g, n_c,-1,p_muta);
	    int g_2 = rele_AG_selezione(pop_g, n_c,g_1,0.4);
	    rele_AG_incrocia(pop_g[g_1],
			     pop_g[g_2],
			     pop_f+2*k,
			     pop_f+2*k+1);
	    // Il primo figlio è il genitore più idoneo
	    memcpy(pop_f+2*k,pop_g+g_1,sizeof(rele_croma));
	    
	  }
	/* Mutazione stocastica */
	p_muta = 0.3;
	if(i_media<10)
	  p_muta = 1.0;
	rele_AG_muta(pop_f, 0.1, p_muta);
	
	
	/* I figli diventano la nuova generazione di genitori */
	memcpy(pop_g,pop_f,n_c*sizeof(rele_croma));
    }
  
  /* Mostra l'istogramma della idoneità della popolazione dei cromosomi */
  rele_AG_stampa_popolazione(pop_f, n_c, "Parabola",1);
    
  /* trasferimento del cromosoma alle sinapsi */
  int scelta = rele_AG_selezione(pop_g, n_c,-1,0);
  rele_AG_trascrivi_sinapsi(pop_f[scelta], r);

  /* Testa la rete */
  casi = 0;
  do
    {
      struct timeval tv;
      gettimeofday(&tv,NULL);
      srand(tv.tv_usec);
      
      /* Genera le cooridnate di un punto nel piano */
      d[0]=-2+(double)rand()/(double)RAND_MAX*4.;
      d[1]=(double)rand()/(double)RAND_MAX*5.;
      c[0] = 1;
      
      if(-d[0]*d[0]+4<d[1])
	{
	  c[0]=0;
	  casi ++;
	}
      rele_Classifica(r, d);
      printf(" %lf,%lf,%d \n",d[0],d[1],r->strato_uscita[0]>0.5);
    }
  while(casi<20);

  /* salva la rete */
  FILE * f= fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
  
  rele_Libera_rete(r);
}
