
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#ifndef VETTORE
#define VETTORE

typedef struct _Vettore {
   double x;
   double y;
} Vettore;

Vettore v_clona(Vettore v1);
Vettore v_sottrai(Vettore v1,Vettore v2);
Vettore v_somma(Vettore v1,Vettore v2);
Vettore v_moltiplica_scalare(Vettore v, double scalare);
Vettore v_normalizza(Vettore v);
double v_prodotto_scalare(Vettore v1,Vettore v2);
double v_modulo(Vettore v);
double v_modulo_quadrato(Vettore v);
void v_inverti(Vettore *v);

#endif
