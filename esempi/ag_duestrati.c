#include "librele.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 50000
#define BATCH_SIZE 550
#define NEUR_SENS 2
#define NEUR_AFF 1
#define N_CROMOSOMI 95
/*
  COMPILAZIONE
  gcc -o ag_uno_strato ag_uno_strato.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  // Cancello schermo
  printf("\x1b[2J");
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
	  
	  /* il punto è classificato come 0 se giace sotto la bisettrice */
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
      i_media=i_media/(double)n_c;
      rele_AG_stampa_popolazione(pop_g, n_c, "Parabola",1);
      
      //printf("Idoneità media %lf\n",i_media);
      if(i_media>25)
	{
	  printf("Iterazione %d\n",i);
	  printf("Idoneità media %lf\n",i_media);
	  break;
	}
      /* Seleziona i cromosomi */
      double p_muta = 0.1;
      if(i_media<10)
	p_muta = 0.9;
      for(int k=0;k<n_u;k++)
	{
	  /* Genera due figli */
	  int g_1 = rele_AG_selezione(pop_g, n_c,-1,p_muta);
	  int g_2 = rele_AG_selezione(pop_g, n_c,g_1,0.3);
	  rele_AG_incrocia(pop_g[g_1],
			   pop_g[g_2],
			   pop_f+2*k,
			   pop_f+2*k+1);
	  memcpy(pop_f+2*k,pop_g+g_1,sizeof(rele_croma));
	  
	}
      
      
      /* Mutazione stocastica */
      p_muta = 0.1;
      if(i_media<10)
	p_muta = 0.9;
      rele_AG_muta(pop_f, 0.1, p_muta);
     
     
      /* I figli diventano la nuova generazione di genitori */
      memcpy(pop_g,pop_f,n_c*sizeof(rele_croma));
      //printf("Iterazione %d\n",i);
    }
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
      //printf(" %lf vs %d \n",c[0],r->strato_uscita[0]>0.5);
      printf(" %lf,%lf,%d \n",d[0],d[1],r->strato_uscita[0]>0.5);
      
      
    }while(casi<20);
  
  
  FILE * f= fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
  
  rele_Libera_rete(r);
}
