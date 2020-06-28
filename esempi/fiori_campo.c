#include "librele.h"
#include <stdio.h>
#include <stdlib.h>

#define EQM_ACCETTABILE 0.0901
#define ITERAZIONI 500

/*
  COMPILAZIONE
  gcc -o fiori_campo fiori_campo.c -I../lib/include ../lib/librele.o -lm
 */

int main()
{
  rele_rete * r =  rele_Crea_rete(2,4,2,0);
  rele_parametri par;
  par.fattore_apprendimento = 0.1255;

  double d[4];
  double c[4];
  int iterazioni = 0;
  double eqm = 100;
  double eqm_old = 100;

  do
    {
      eqm_old =  eqm;
      eqm = 0;
      for(int i=0;i<ITERAZIONI;i++)
	{
	  double x,y;
	  x = (double)rand()/(double)RAND_MAX;
	  y = (double)rand()/(double)RAND_MAX;
	  c[0] = 0;
	  c[1] = 0;
	  c[2] = 0;
	  c[3] = 0;
	  d[0] = x;
	  d[1] = y;
	  if(y>x && y<(1-x)) c[0] = 1;
	  if(y<x && y<(1-x)) c[1] = 1;
	  if(y<x && y>(1-x)) c[2] = 1;
	  if(y>x && y>(1-x)) c[3] = 1;
	  r = rele_Addestra(r,&par,d,c);
	  eqm += 1./ITERAZIONI*r->EQ;
	}
      printf("%lf->%lf\n",eqm,eqm_old);
    }
  while( eqm-1<eqm_old && eqm>EQM_ACCETTABILE);
  printf("Chiudo a %lf vs %lf\n",eqm,eqm_old);

  FILE * f = fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
  
  printf("Terminato in %d iterazioni\n",iterazioni);
  for(int i=0;i<1000;i++)
    {
      d[0]=1.*(double)rand()/(double)RAND_MAX;
      d[1]=1.*(double)rand()/(double)RAND_MAX;
      rele_Classifica(r, d);
      int etichetta = 0;
      double *e = r->strato_uscita;
      if(e[0]>0.5 && e[0]>e[1] && e[0]>e[2] && e[0]>e[3])
	printf(" %lf %lf %d\n",d[0],d[1],1);
      
      if(e[1]>0.5 && e[1]>e[0] && e[1]>e[2] && e[1]>e[3])
	printf(" %lf %lf %d\n",d[0],d[1],2);
      if(e[2]>0.5 && e[2]>e[1] && e[2]>e[0] && e[2]>e[3])
	printf(" %lf %lf %d\n",d[0],d[1],3);
      if(e[3]>0.5 && e[3]>e[1] && e[3]>e[2] && e[3]>e[0])
	printf(" %lf %lf %d\n",d[0],d[1],4);
      
    }
  rele_Libera_rete(r);
}
