#include "librele.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 100.

/*
  COMPILAZIONE
  gcc -o due_strati due_strati.c -I../lib/include ../lib/librele.o -lm
 */

/*
 * Addestra una rete con la backpropagation
 * trasferisce le sinapsi in un cromosoma
 * generea una nuova reta dal cromosoma
 */
int main()
{
  rele_rete * r =  rele_Crea_rete( 2,1,30,0);
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
	   d[0]=4.*(double)rand()/(double)RAND_MAX-2;
	   d[1]=5*(double)rand()/(double)RAND_MAX;
	   c[0]=1;
	  /* il punto è classificato come 0 se giace sotto la parabola */
	  if(-d[0]*d[0]+4 < d[1]) c[0]=0;
	  /* addestra la rete passando le coordinate del punto e il target
	     output, cioè la label della classe di appartenenza */
	  r = rele_Addestra(r,&par,d,c);
	  eqm+=1./ITERAZIONI*r->EQ;
	}
    }
  /* Termina quando l'errore quadratico, mediato su 100 iterazioni è soddisfacente */
  while(eqm>EQM_ACCETTABILE);


  /*
   * Trasferisce le sinapsi
   */
  rele_croma * croma = rele_AG_Crea_cromosomi(r,1);
  memcpy(croma->gene,r->v_t,croma->n_geni_l1*sizeof(allele));
  memcpy(croma->gene+croma->n_geni_l1,r->v_u,croma->n_geni_l2*sizeof(allele));


  
  rele_rete * r2 =  rele_Crea_rete( 2,1,30,0);

  rele_AG_trascrivi_sinapsi(*croma, r2);

  
  printf("Terminato in %d iterazioni\n",iterazioni);
  for(int i=0;i<1000;i++)
    {
      d[0]=4.*(double)rand()/(double)RAND_MAX-2;
      d[1]=5*(double)rand()/(double)RAND_MAX;
      rele_Classifica(r2, d);
      printf(" %lf %lf %d\n",d[0],d[1],r2->strato_uscita[0]>0.5);
    }
  rele_Libera_rete(r);
  rele_Libera_rete(r2);
}
