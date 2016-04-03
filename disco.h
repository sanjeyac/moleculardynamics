
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vettore.h"


#ifndef DISCO
#define DISCO

typedef struct _Disco {
   int id;//identificativo
   Vettore pos;//vettore di posizione
   Vettore acc;//vettore accelarazione
   Vettore vel;//vettore velocita
   double raggio; //raggio
   double massa; //massa
} Disco;

void d_inizializza(Disco *disco, double larghezzaMax, double h);
void d_print(Disco disco);
double d_distanza(Disco disco1, Disco disco2);
int d_is_collisione(Disco disco1, Disco disco2);
Disco d_predici_posizione(Disco disco, double tempo);

//ricerca collisioni
Disco* d_trova_collisione( Disco disco, Disco *dischi, int max);
Disco d_correggi_predizione(Disco originale,Disco urtato,double tempo,double smorzamento);
Disco d_collisione_esatta(Disco originale,Disco urtato,double tempo,double smorzamento);
int d_is_muovibile(Disco disco, Disco *dischi, int n,double tempo);
void d_limite_velocita(Disco *disco);

//utils
double risoluzioneMotoAccelerato(double a,double b,double c);

#endif
