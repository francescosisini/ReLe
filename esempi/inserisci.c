#include "libcallidus.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
  Immobile imm;
  imm.area = atoi(argv[1]);
  imm.piani = atoi(argv[2]);
  imm.bagni = atoi(argv[3]);
  imm.giardino = atoi(argv[4]);
  imm.distanza_centro = atoi(argv[5]);
  imm.vani = atoi(argv[6]);
  imm.tipo_r = atoi(argv[7]);
  imm.lati_indipendenti = atoi(argv[8]);

  int chiave_imm = db_inserisci_immobile(&imm);
  printf("inserito: %d\n", chiave_imm);

  Immobile clau,record;
  db_inizializza_immobile(&clau);
  clau.bagni = 1;
  int id = 0;
  do{
    
    id = db_seleziona_immobile(&record,clau,id);
    printf("Trovato %d\n",id);
    
  }
  while(id>0);
  Immobile up;
  up = imm;
  up.id = 5;
  up.bagni = 255;
  db_aggiorna_immobile(up);
  
}
