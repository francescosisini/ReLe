#include "librele.h"
#include <stdio.h>
#include <stdlib.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 300.

/*
  COMPILAZIONE
  gcc -o ag_uno_strato ag_uno_strato.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  rele_rete * r =  rele_Crea_rete(2,1,0,0);

  /* crea due popolazioni di 100 cromosomi: genitori e figli */
  int n_c = 100;
  int n_u = n_c/2; //N_c deve essere pari
  rele_croma * pop_g = rele_AG_Crea_cromosomi(r, n_c);
  rele_croma * pop_f = rele_AG_Crea_cromosomi(r, n_c);
  
  double d[2];
  double c[1];
  
  int iterazioni = 0;
  double eqm;
  
  for(int i=0;i<ITERAZIONI;i++)
    {
      
      /* Genera le cooridnate di un punto nel piano */
      d[0]=(double)rand()/(double)RAND_MAX;
      d[1]=(double)rand()/(double)RAND_MAX;
      c[0]=1;
      /* il punto è classificato come 0 se giace sotto la bisettrice */
      if(d[0]<d[1]*2.) c[0]=0;
      /* calcola l'ideneità di ogni cromosoma */
      for(int k=0; k<n_c; k++)
	{
	  rele_AG_Calcola_idoneita_cromosoma(pop_g+k,r,d,c);
	}
      for(int k=0;k<n_u;k++)
	{
	  int g_1 = rele_AG_selezione(pop_g, n_c,-1);
	  int g_2 = rele_AG_selezione(pop_g, n_c,g_1);
	  rele_AG_incrocia(pop_g[g_1],
			   pop_g[g_2],
			   pop_f+2*k,
			   pop_f+2*k+1);
	}
      
      
    }
  /* trasferimento del cromosoma alle sinapsi */
  rele_AG_trascrivi_sinapsi(pop_f[0], r);

  FILE * f= fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
    
  rele_Libera_rete(r);
}
