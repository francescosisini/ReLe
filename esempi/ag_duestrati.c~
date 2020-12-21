#include "librele.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 10
#define BATCH_SIZE 200
#define NEUR_SENS 2
#define NEUR_AFF 1
#define N_CROMOSOMI 100
/*
  COMPILAZIONE
  gcc -o ag_uno_strato ag_uno_strato.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  rele_rete * r =  rele_Crea_rete(NEUR_SENS,NEUR_AFF,0,0);

  /* crea due popolazioni di n_c cromosomi: genitori e figli */
  int n_c = N_CROMOSOMI;
  int n_u = n_c/2; //N_c deve essere pari
  rele_croma * pop_g = rele_AG_Crea_cromosomi(r, n_c);
  rele_croma * pop_f = rele_AG_Crea_cromosomi(r, n_c);

  /* dati addestramento */
  double d[NEUR_SENS*BATCH_SIZE];
  double c[NEUR_AFF*BATCH_SIZE];
  double m = 1./10.;

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
	  d[0+NEUR_SENS*casi]=-5+(double)rand()/(double)RAND_MAX*10.;
	  d[1+NEUR_SENS*casi]=-5+(double)rand()/(double)RAND_MAX*10;
	  
	  /* il punto è classificato come 0 se giace sotto la bisettrice */
	  if(d[0+NEUR_SENS*casi]<d[1+NEUR_SENS*casi]*m)
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
      /*dati e classi printi */
      while(casi<(b.numero-1));
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
      printf("Idoneità media %lf\n",i_media);

      /* Seleziona i cromosomi */
      for(int k=0;k<n_u;k++)
	{
	  /* Genera due figli */
	  int g_1 = rele_AG_selezione(pop_g, n_c,-1);
	  int g_2 = rele_AG_selezione(pop_g, n_c,g_1);
	  rele_AG_incrocia(pop_g[g_1],
			   pop_g[g_2],
			   pop_f+2*k,
			   pop_f+2*k+1);
	}
      
      /* I figli diventano la nuova generazione di genitori */
      memcpy(pop_g,pop_f,n_c*sizeof(rele_croma));
      printf("Iterazione %d\n",i);
    }
  
  /* trasferimento del cromosoma alle sinapsi */
  int scelta = rele_AG_selezione(pop_g, n_c,-1);
  rele_AG_trascrivi_sinapsi(pop_f[scelta], r);

  /* Testa la rete */
  
  casi = 0;
  do
    {
      struct timeval tv;
      gettimeofday(&tv,NULL);
      srand(tv.tv_usec);
      
      
      d[0]=-5+(double)rand()/(double)RAND_MAX*10.;
      d[1]=-5+(double)rand()/(double)RAND_MAX*10;
      c[0]=1;
      
      
      if(d[0]<d[1]*m)
	{
	  c[0]=0;
	  casi ++;
	}
      rele_Classifica(r, d);
      printf(" %lf vs %d \n",c[0],r->strato_uscita[0]>0.5);
      
      
    }while(casi<20);
  
  
  FILE * f= fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
  
  rele_Libera_rete(r);
}
