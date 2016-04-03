#include "vettore.h"
#include <math.h>

Vettore v_clona(Vettore v2){
  Vettore v1;
  v1.x = v2.x;
  v1.y = v2.y;
  return v1;
}

double v_modulo(Vettore v){
  return sqrt( v.x*v.x + v.y*v.y );
}

double v_modulo_quadrato(Vettore v){
   return v.x*v.x + v.y*v.y;
}

Vettore v_somma(Vettore v1,Vettore v2){
  Vettore result;
  result.x = v1.x + v2.x;
  result.y = v1.y + v2.y;
  return result;
}

Vettore v_normalizza(Vettore v){
  Vettore result;
  double modulo = v_modulo(v);
  result.x = v.x / modulo;
  result.y = v.y /modulo;
  return result;
}

Vettore v_sottrai(Vettore v1,Vettore v2){
  Vettore result;
  result.x = v1.x - v2.x;
  result.y = v1.y - v2.y;
  return result;
}

Vettore v_moltiplica_scalare(Vettore v, double scalare){
  Vettore result;
  result.x = v.x * scalare;
  result.y = v.y * scalare;
  return result;
}

double v_prodotto_scalare(Vettore v1,Vettore v2){
  return (v1.x * v2.x) + (v1.y * v2.y);
}

void v_inverti(Vettore *v){
  v->x = -v->x;
  v->y = -v->y;
}

void v_print(Vettore v){
  printf("vector (%f,%f)\n", v.x, v.y );
}
