#include "librele.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 100

/*
  COMPILAZIONE
  gcc -o ag_uno_strato ag_uno_strato.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  rele_rete * r =  rele_Crea_rete(2,1,0,0);

  /* crea due popolazioni di 100 cromosomi: genitori e figli */
  int n_c = 5000;
  int n_u = n_c/2; //N_c deve essere pari
  rele_croma * pop_g = rele_AG_Crea_cromosomi(r, n_c);
  rele_croma * pop_f = rele_AG_Crea_cromosomi(r, n_c);
  
  double d[2];
  double c[1];
  
  int iterazioni = 0;
  double eqm;

  srand(time(0));
  int casi = 0;
  double id_media = 0;
  int classe_precedente = 0;
  do
    {
      //printf("ITERAZIONE %d\n",i);
      /* Genera le cooridnate di un punto nel piano */
      d[0]=(double)rand()/(double)RAND_MAX;
      d[1]=(double)rand()/(double)RAND_MAX;
      
      /* il punto è classificato come 0 se giace sotto la bisettrice */
      if(d[0]<d[1]*2)
	{
	  c[0]=0;
	}
      else
	{
	  c[0]=1;
	}
      if(c[0] == classe_precedente)
	continue;
      casi++;
      printf("Casi %d\n",casi);
      
      /* calcola l'ideneità di ogni cromosoma */
      id_media = 0;
      for(int k=0; k<n_c; k++)
	{
	  
	  rele_AG_Calcola_idoneita_cromosoma(pop_g+k,r,d,c);
	  //printf("Cromosoma %d, idoneità %lf\n",k,pop_g[k].idoneita);
	  id_media += pop_g[k].idoneita;
	}
      printf("G Idoneità media %lf\n",id_media/(double)n_c);
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
      for(int k=0; k<n_c; k++)
      	{
	  
	  rele_AG_Calcola_idoneita_cromosoma(pop_g+k,r,d,c);
	  //printf("Cromosoma %d, idoneità %lf\n",k,pop_g[k].idoneita);
	  id_media += pop_g[k].idoneita;
	}
      printf("F Idoneità media %lf\n",id_media/(double)n_c);
      
    }while(casi<150);
  
  /* trasferimento del cromosoma alle sinapsi */
  rele_AG_trascrivi_sinapsi(pop_g[0], r);

  /* Testa la rete */
  casi = 0;
  do
    {
      struct timeval tv;
      gettimeofday(&tv,NULL);
      srand(tv.tv_usec);
      //printf("ITERAZIONE %d\n",i);
      /* Genera le cooridnate di un punto nel piano */
      d[0]=(double)rand()/(double)RAND_MAX;
      d[1]=(double)rand()/(double)RAND_MAX;
      c[0]=1;
      
      /* il punto è classificato come 0 se giace sotto la bisettrice */
      if(d[0]<d[1])
	{
	  c[0]=0;
	  casi ++;
	}
      rele_Classifica(r, d);
      printf(" %lf vs %lf \n",c[0],r->strato_uscita[0]);
      /* calcola l'ideneità di ogni cromosoma */
      
    }while(casi<20);
  
  
  
  
  FILE * f= fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
  
  rele_Libera_rete(r);
}
