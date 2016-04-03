#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "vettore.h"
#include "contenitore.h"
#include "disco.h"
#include "const.h"



int main(){

    /**
  Vettore vettore;
  vettore.x = 3.0f;
  vettore.y = 3.0f;
  double modulo =  v_modulo(vettore);
  printf("%f\n", modulo );

  Contenitore contenitore;
  contenitore.bordo_destro    = 10.0f;
  contenitore.bordo_sinistro  = 0.0f;
  contenitore.bordo_inferiore = 0.0f;

  Disco disco;
  d_inizializza(&disco,10.0f);
  disco.pos.x = 5.0f;
  disco.pos.y = 5.0f;
  disco.raggio = 1.0f;

  double d = c_distanza( disco, contenitore );
  printf("%f\n", d );

  Disco res = c_collisione_esatta(disco,contenitore,0.01f);


  Disco dischi[2];

  d_inizializza(&dischi[0],10.0f);
  dischi[0].pos.x = 0.0f;
  dischi[0].pos.y = 0.0f;
  dischi[0].raggio = 1.0f;
  dischi[0].id = 0;

  d_inizializza(&dischi[1],10.0f);
  dischi[1].pos.x = 2.0f;
  dischi[1].pos.y = 2.0f;
  dischi[1].raggio = 1.0f;
  dischi[1].id = 1;

  disco.pos.x = 1.0f;
  disco.pos.y = 1.5f;
  disco.raggio = 1.0f;
  disco.id = 2;

  Disco *colliso = d_trova_collisione( disco, dischi, 2 );
  printf("%d\n", colliso->id);
  /**/

  return 0;
}
