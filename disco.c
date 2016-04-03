#include "disco.h"
#include "const.h"
#include "vettore.h"
#include "assert.h"
#include <time.h>
#include <stdlib.h>
#include <math.h>

Disco d_correggi_predizione(Disco originale,Disco urtato,double tempo,double smorzamento){

    double distanza = d_distanza( originale, urtato );
    double scarto =  distanza - originale.raggio*2;

    //se lo scarto è > 0 non si toccano, se lo scarto è < si toccano
    //assert( scarto > 0.0f );

    double t = -1.0f;
    Disco collisioneEsatta;
    if ( scarto > 0.00001f ){

        if ( fabs(originale.vel.x) > 0.0000001f ){
            t = scarto / fabs(originale.vel.x);
        } else if ( fabs(originale.vel.y) > 0.0000001f ){
            double a = 0.5f*originale.acc.y;
            double b = originale.vel.y;
            double c = scarto;
            t = risoluzioneMotoAccelerato(a,b,c);
        }
        assert(t>0.0f);

        collisioneEsatta = d_predici_posizione(originale,t);
        d_print(collisioneEsatta);
        double newdist = d_distanza( originale, urtato );
        printf("posizione del disco in urto con altro disco | scarto: %f | new: %f\n", scarto, newdist );

        assert( collisioneEsatta.pos.y > 1 );

    }
    else{
        collisioneEsatta = originale;
    }


      /**/
      // First, find the normalized vector n from the center of
      // circle1 to the center of circle2
      Vettore n =  v_sottrai( collisioneEsatta.pos, urtato.pos );
      n =  v_normalizza( n );

      Vettore v1 = collisioneEsatta.vel;
      Vettore v2; v2.x = 0.0f; v2.y = 0.0f;
      // Find the length of the component of each of the movement
      // vectors along n.
      // a1 = v1 . n
      // a2 = v2 . n
      float a1 = v_prodotto_scalare(v1,n);
      float a2 = v_prodotto_scalare(v2,n);

      double m1 = 1.0f;
      double m2 = 1.0f;

      // Using the optimized version,
      // optimizedP =  2(a1 - a2)
      //              -----------
      //                m1 + m2
      double optimizedP = (2.0 * (a1 - a2)) / (m1 + m2);

      // Calculate v1', the new movement vector of circle1
      // v1' = v1 - optimizedP * m2 * n
      Vettore v1after = v_sottrai(v1 , v_moltiplica_scalare(n,optimizedP * m2) );

      // Calculate v1', the new movement vector of circle1
      // v2' = v2 + optimizedP * m1 * n
      Vettore v2after = v_sottrai(v2 , v_moltiplica_scalare(n,optimizedP * m1) );

      if (
            fabs( collisioneEsatta.vel.x  - v1after.x )<0.0001f &&
            fabs( collisioneEsatta.vel.y  - v1after.y )<0.0001f
        ){
                v1after = v_moltiplica_scalare( collisioneEsatta.vel, -smorzamento );
        }
        collisioneEsatta.vel = v1after;

    return collisioneEsatta;
}


double risoluzioneMotoAccelerato(double a,double b,double c){
    double discriminante, sqrtDisc, t1,t2, t;

    discriminante = b*b - 4*a*c;
    sqrtDisc = sqrt( discriminante );
    t1 = ( -b + sqrtDisc )/  (2*a);
    t2 = ( -b - sqrtDisc )/  (2*a);

    t = -1.0f;

    if (t1>0) t = t1;
    if (t2>0) t = t2;

    if ( t1 > 0 && t2 > 0 ){
        if ( t1 > t2)
        t=t2;
        else
        t=t1;
    }

    return t;
}

void d_limite_velocita(Disco *disco){
    if ( fabs(disco->vel.x) < 0.0001f ){
        disco->vel.x = 0.0f;
    }
    if ( fabs(disco->vel.y) < 0.0001f ){
        disco->vel.y = 0.0f;
    }
}

int d_is_muovibile(Disco disco, Disco *dischi, int n, double tempo){

    //se la velocità è bassa e sto urtando un disco allora sto fermo
    //Disco
    Disco predetto = d_predici_posizione(disco,tempo);
    Disco *collisione = d_trova_collisione(predetto,dischi,n);
    double vel = v_modulo(disco.vel);

    // se il disco scende sotto la soglia del container non va bene
    //assert( disco.pos.y >= 0 );

    return !( vel < VEL_MINIMA_FERMA && collisione != NULL );

}

//controllo senza usare la radice quadrata ( onerosa )
int d_is_collisione(Disco disco1, Disco disco2){
    double distanza = d_distanza(disco1,disco2);
    double distanzaMinima = disco1.raggio + disco2.raggio;
    return distanza < distanzaMinima;
}

Disco d_predici_posizione(Disco disco, double tempo){

    assert( tempo > 0.0f );
    Disco predetto;

    //applicazione leggi orarie
    predetto.vel.x = disco.vel.x + disco.acc.x * tempo;
    predetto.pos.x = 0.5f * disco.acc.x * tempo * tempo + disco.vel.x * tempo + disco.pos.x;
    predetto.vel.y = disco.vel.y + disco.acc.y * tempo;
    predetto.pos.y = 0.5f * disco.acc.y * tempo * tempo + disco.vel.y * tempo + disco.pos.y;

    //questo if evita un loop continuo
    if ( fabs( disco.pos.y - disco.raggio) < 0.00001f && fabs( disco.vel.y) < 0.00001f ){
        predetto.vel.y = disco.vel.y;
        predetto.pos.y = disco.pos.y;
    }

    predetto.acc = disco.acc;
    predetto.raggio = disco.raggio;
    predetto.massa = disco.massa;
    predetto.id = disco.id;

    return predetto;
}

/*
Vettore projectUonV(Vettore u, Vettore v) {
Vec3 r;
r = v_moltiplica_scalare(v, v_prodotto_scalare(u, v) / v_prodotto_scalare(v, v));
return r;
}
*/


int contatore = 0;

Disco d_collisione_esatta(Disco originale,Disco urtato,double tempo,double smorzamento){
    contatore++;

    assert(originale.pos.y >= 0);
    if ( !(tempo>0.0f) ){ //questo perchè non posso fare == 0.0f
        return originale;
    }
    assert( tempo > 0.0f );

    //predici prossimo avvicinamento al contenitore
    Disco predetto = d_predici_posizione(originale,tempo);

    //controlla collisione
    if ( d_is_collisione(predetto, urtato) ){
        Vettore v_distanza = v_sottrai( predetto.pos, urtato.pos );
        double distanza = v_modulo( v_distanza );

        //se ricorsine terminata ritorna il valore
        if ( fabs( distanza - predetto.raggio*2 ) < TOLLERANZA_COLLISIONE ){
            return predetto;
        }

        // se c'è stata una collissione torna indietro e riprova con la metà del passo
        return d_collisione_esatta( originale,urtato,tempo/2,smorzamento );
    }else{
        return d_collisione_esatta( predetto, urtato, tempo, smorzamento );
    }

}

void d_inizializza(Disco *disco, double larghezzaMax,double h){
    disco->acc.y = -9.81f;
    disco->raggio = 1.0f;
    disco->pos.x = disco->raggio+((double)rand()/(double)RAND_MAX)*(larghezzaMax);
    disco->pos.y = h;
}

void d_print(Disco disco){
    printf("ID:%d   p(%f,%f)   v(%f,%f)   a(%f,%f)   ",
    disco.id,disco.pos.x,disco.pos.y,disco.vel.x,disco.vel.y,disco.acc.x,disco.acc.y);
}

double d_distanza(Disco disco1, Disco disco2){
    Vettore distanza = v_sottrai( disco1.pos, disco2.pos );
    return v_modulo( distanza );
}

int d_is_collissione( Disco d1, Disco d2 ){
    double distanza = d_distanza( d1, d2 );
    return distanza < 2*d1.raggio;
}

//ricerca collisioni
Disco* d_trova_collisione( Disco disco, Disco *dischi, int max){

    for ( int i=0; i < max ; i++ ){
        Disco selezionato = dischi[i];
        if ( d_is_collissione(disco, selezionato) ){
            return &dischi[i];
        }
    }

    return NULL;
}

//ricalcolo post cos
void d_post_collisione( Disco disco, Disco discoUrtato, double smorzamento){
    v_inverti( &disco.vel );
}
