/*
  crea_salva_apri
  da introduzione alle reti neurali di Francesco Sisini
  @2020 Francesco e Valentina Sisini
 */
#include "librele.h"
#include <stdio.h>
#include <stdlib.h>


int main()
{
  rele_rete * r =  rele_Crea_rete(2,1,0,0);
  rele_parametri par;

  FILE * f = fopen("rete.csv","wt");
  rele_Salva(r,f);
  fclose(f);
  rele_Libera_rete(r);
  
  f= fopen("rete.csv","rt");
  r = rele_Apri(f);
  
  rele_Libera_rete(r);
}



