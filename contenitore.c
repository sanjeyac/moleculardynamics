#include "contenitore.h"
#include "const.h"
#include "assert.h"
#include "stdio.h"

void c_inizializza(Contenitore *c, double sx, double dx, double inf){
    c->bordo_destro = dx;
    c->bordo_sinistro = sx;
    c->bordo_inferiore = inf;
}

int rimbalzi = 0;

Disco c_correggi_predizione(Disco disco, Contenitore contenitore, double tempo, double smorzamento ){
    assert( tempo > 0.0f );

    Disco collisioneEsatta;

    printf("ricerco la collissione esatta col contenitore\n" );
    Disco predetto = d_predici_posizione( disco, tempo );
    enum CollisioneBordo collisioneConBordo = c_trova_collisione(predetto, contenitore);
    double distanzaContenitore = c_distanza( disco, contenitore );
    double scarto = fabs( distanzaContenitore - predetto.raggio );
    double tempoAllaCollisione = -1.0f;

    double a,b,c,t1,t2,t,discriminante,sqrtDisc;

    switch (collisioneConBordo) {

        case COLLISIONE_BORDO_DESTRO:
        case COLLISIONE_BORDO_SINISTRO:
            if ( scarto > 0.00001f ){
                tempoAllaCollisione = scarto/(fabs(disco.vel.x));
                printf("scarto %f, tempo alla collissione %f\n",scarto, tempoAllaCollisione );
                assert(tempoAllaCollisione>0.0f);
                collisioneEsatta = d_predici_posizione( disco, tempoAllaCollisione );
            }else{
                collisioneEsatta = disco;
            }
            collisioneEsatta.vel.x = -collisioneEsatta.vel.x * smorzamento;
            return collisioneEsatta;
            break;

        case COLLISIONE_BORDO_INFERIORE:
            if ( scarto > 0.00001f ){
                // moto accelerato
                //y = y0 + v*t + 1/2 * a * t^2
                a = 0.5f*disco.acc.y;
                b = disco.vel.y;
                c = scarto;
                t = risoluzioneMotoAccelerato(a,b,c);
                assert(t>0.0f);
                collisioneEsatta = d_predici_posizione( disco, t );
            }else{
                collisioneEsatta = disco;
            }

            d_print( disco );
            printf("| <- partendo da \n" );
            d_print(collisioneEsatta);
            printf("| posizione calcolata\n" );

            collisioneEsatta.vel.y = -collisioneEsatta.vel.y * smorzamento;

            return collisioneEsatta;
            break;
    }
}


//funzione ricorsiva
Disco c_collisione_esatta(Disco disco, Contenitore contenitore, double tempo ){

    assert(disco.pos.y >= 0);
    // if ( !(tempo>0.0f) ){ //questo perchè non posso fare == 0.0f
    //   return originale;
    // }
    assert( tempo > 0.0f );


    //predici prossimo avvicinamento al contenitore
    Disco predetto = d_predici_posizione(disco,tempo);
    double distanzaContenitore = c_distanza( predetto, contenitore );
    double scarto = fabs( distanzaContenitore - predetto.raggio );
    d_print(predetto);
    printf("c_collisione_esatta %f | in ricerca collissione esatta | ",scarto);
    //controlla collisione


    if ( c_trova_collisione( predetto, contenitore ) == NESSUNA_COLLISIONE ){
        // se non c'è nessuna collissione prova a vedere se c 'è lo scarto minimo
        if ( scarto < TOLLERANZA_COLLISIONE ){
            printf(" trovato\n");
            return predetto;
        }

        printf(" avanti\n");
        // prova ad avanzare ancora
        return c_collisione_esatta( predetto, contenitore, tempo );
    }else{
        printf(" indietro\n");
        //se c'è stata una collissione torna indietro e prova un passo più corto
        return c_collisione_esatta( disco, contenitore, tempo/2.0f );
    }

}


int c_is_muovibile(Disco disco, Contenitore contenitore ){
    double vel = v_modulo(disco.vel);
    double distanzaContenitore = c_distanza( disco, contenitore );
    double scarto = fabs( distanzaContenitore - disco.raggio );
    // se il disco scende sotto il container non va bene
    //assert( disco.pos.y >= 0 );
    return !( vel < VEL_MINIMA_FERMA && scarto<TOLLERANZA_COLLISIONE );
}

//devo minimizzare questa funzione finechè non raggiunge lo scarto minimo irhchiesto
double c_distanza( Disco disco, Contenitore contenitore ){
    double sx  = fabs( contenitore.bordo_sinistro - disco.pos.x );
    double dx  = fabs( contenitore.bordo_destro - disco.pos.x );
    double inf = fabs( disco.pos.y - contenitore.bordo_inferiore );

    double ret = -1.0f;

    if ( sx < dx && sx < inf  ){
        return sx;
    }

    if ( dx < sx && dx < inf  ){
        return dx;
    }

    if ( inf < sx && inf < dx  ){
        return inf;
    }

    return inf;
}


int c_trova_collisione( Disco disco, Contenitore contenitore ){
    //distanza col bordo sinistro dal centro del disco
    double sx  = fabs( contenitore.bordo_sinistro - disco.pos.x );

    //distanza col bordo destro dal centro del disco
    double dx  = fabs( contenitore.bordo_destro - disco.pos.x );

    //distanza col bordo inferioe dal centro del disco
    double inf = fabs( disco.pos.y - contenitore.bordo_inferiore );
    double r = disco.raggio;

    if ( sx < r  ){
        return COLLISIONE_BORDO_SINISTRO;
    }
    if (  dx < r ){
        return COLLISIONE_BORDO_DESTRO;
    }
    if ( (disco.pos.y < 0.0f || inf < r ) && fabs( disco.vel.y ) > 0.000001f  ){
        return COLLISIONE_BORDO_INFERIORE;
    }
    return NESSUNA_COLLISIONE;
}
