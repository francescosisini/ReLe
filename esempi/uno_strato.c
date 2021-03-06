#include "librele.h"
#include <stdio.h>
#include <stdlib.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 100.

/*
  COMPILAZIONE
  gcc -o uno_strato uno_strato.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  rele_rete * r =  rele_Crea_rete(2,1,0,0);
  rele_parametri par;
  par.fattore_apprendimento = 0.05;
  
  double d[2];
  double c[1];
  
  int iterazioni = 0;
  double eqm;
  do
    {
      eqm = 0;
      for(int i=0;i<ITERAZIONI;i++)
	{
	  iterazioni++;
	  /* Genera le cooridnate di un punto nel piano */
	  d[0]=(double)rand()/(double)RAND_MAX;
	  d[1]=(double)rand()/(double)RAND_MAX;
	  c[0]=1;
	  /* il punto è classificato come 0 se giace sotto la bisettrice */
	  if(d[0]<d[1]*2.) c[0]=0;
	  /* addestra la rete passando le coordinate del punto e il target
	     output, cioè la label della classe di appartenenza */
	  r = rele_Addestra(r,&par,d,c);
	  eqm+=1./ITERAZIONI*r->EQ;
	}
    }
  /* Termina quando l'errore quadratico, mediato su 100 iterazioni è soddisfacente */
  while(eqm>EQM_ACCETTABILE);

  FILE * f= fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
    
  rele_Libera_rete(r);
}
