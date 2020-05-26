#include "librele.h"
#include <stdio.h>
#include <stdlib.h>

#define EQM_ACCETTABILE 0.01
#define ITERAZIONI 100.


/*
  COMPILAZIONE
  gcc -o tre_strati_b tre_strati_b.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  rele_rete * r =  rele_Crea_rete(2,2,20,20);
  rele_parametri par;
  par.fattore_apprendimento = 0.3;
  
  double d[2];
  double c[2];
  
  int iterazioni = 0;
  double eqm;
  do
    {
      eqm = 0;
      for(int i=0;i<ITERAZIONI;i++)
	{
	  iterazioni++;
	  /* Genera le cooridnate di un punto nel piano */
	   d[0]=8.*(double)rand()/(double)RAND_MAX-4;
	   d[1]=8.*(double)rand()/(double)RAND_MAX-4;
	   c[0]=0;
	   c[1]=0;
	  /* il punto è classificato come 1 se appartiene alla corono circolare */
	   if((d[0]*d[0] + d[1]*d[1] >1) && (d[0]*d[0] + d[1]*d[1] < 3.)) c[0] = 1 ;
	   if((d[0]*d[0] + d[1]*d[1] >4) && (d[0]*d[0] + d[1]*d[1] < 5.)) c[1] = 1 ;
	   /* addestra la rete passando le coordinate del punto e il target
	      output, cioè la label della classe di appartenenza */
	   r = rele_Addestra(r,&par,d,c);
	   eqm+=1./ITERAZIONI*r->EQ;
	}
    }
  /* Termina quando l'errore quadratico, mediato su 100 iterazioni è soddisfacente */
  while(eqm>EQM_ACCETTABILE);

  printf("Terminato in %d iterazioni\n",iterazioni);
  for(int i=0;i<1000;i++)
    {
      d[0] = 8.*(double)rand()/(double)RAND_MAX-4;
      d[1] = 8.*(double)rand()/(double)RAND_MAX-4;
      rele_Classifica(r, d);
      int cls = 0;
      if(r->strato_uscita[0]>r->strato_uscita[1] && r->strato_uscita[0]> 0.5)
	cls = 1;
      if(r->strato_uscita[1]>r->strato_uscita[0] && r->strato_uscita[1]> 0.5)
	cls = 2;
      printf(" %lf %lf %d\n",d[0],d[1],cls);
    }
  rele_Libera_rete(r);
}
