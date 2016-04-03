
#include <stdio.h>
#include <stdlib.h>
#include "disco.h"

#ifndef CONTENITORE
#define CONTENITORE

enum CollisioneBordo {
  NESSUNA_COLLISIONE,
  COLLISIONE_BORDO_INFERIORE,
  COLLISIONE_BORDO_DESTRO,
  COLLISIONE_BORDO_SINISTRO
};

typedef struct _Contenitore {
   double bordo_sinistro;
   double bordo_destro;
   double bordo_inferiore;
} Contenitore;

Disco c_correggi_predizione(Disco disco, Contenitore contenitore,double tempo, double smorzamento);
void c_inizializza(Contenitore *c, double sx, double dx, double inf);
int c_trova_collisione( Disco disco, Contenitore contenitore );
Disco c_collisione_esatta(Disco disco, Contenitore contenitore, double tempo );
double c_distanza( Disco disco, Contenitore contenitore );
int c_is_muovibile(Disco disco, Contenitore contenitore );

#endif
