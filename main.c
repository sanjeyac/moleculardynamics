#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include "vettore.h"
#include "contenitore.h"
#include "disco.h"
#include "const.h"


/******************************************************************************

MAIN
====

instanzia N dischi

per ogni disco:

predici posizione con un avanzamento di deltat

controlla se c'è una collissione nella posizione predetta

-se collissione con bordo
ricalcola momento esatto della collissione disco e la posizione esatta
ricalcola le nuove direzioni post urto
( inverti direzione della componente)

-se collissione con disco1
ricalcola il momento esatto della collisione
col bordo e la posizione esatta
ricalcola le nuove direzioni post urto

controlla se si è fermato, se si fissa e si passa al prossimo disco1

una volta fissati tutti i dischi
calcolare l'area occupata dalle sfere
e calcolare l'area del contenitore

=================
ottimizzazioni
- griglia di collissione
- controllo distanza - evitare la sqrt

******************************************************************************/

int main(){

    //seed per la generazione di numeri casuali
    srand((unsigned)time(NULL));

    const int max = 300;
    const double delta_t = 0.01f; //secondi
    const double smorzamento = 0.8f;

    Contenitore contenitore;
    contenitore.bordo_sinistro = 0.0f;
    contenitore.bordo_destro = 30.0f;
    contenitore.bordo_inferiore = 0.0f;

    FILE *fp;
    fp = fopen("log.txt", "w");

    //controlla che il file si sia aperto correttamente altrimenti esci
    assert( fp!=NULL );

    Disco dischi[max];

    int n_cycle = 0;


    //cicla su tutti i dischi
    for ( int i = 0; i<max; i++ ){

        Disco disco = dischi[i];
        d_inizializza( &disco, contenitore.bordo_destro-2, 100 );
        disco.id = i;


        do{ //itera finchè il disco non si ferma

            assert( disco.pos.y>0 );

            printf("\n\n\n" );
            d_print(disco);
            printf("main| INIZIO CICLO\n" );
            Disco predetto = d_predici_posizione( disco, delta_t );

            //collissione con contenitore
            enum CollisioneBordo collisioneConBordo = c_trova_collisione(predetto, contenitore);
            d_print(predetto);
            printf("main| <- predizone completata | check container: %d \n", collisioneConBordo);

            if ( collisioneConBordo != NESSUNA_COLLISIONE ){ //se diverso da NESSUNA_COLLLISIONE
                d_print(disco);
                printf("main| COLLISIONE INDIVIDUATA collisione con container, ora correggo\n");
                predetto = c_correggi_predizione( disco, contenitore, delta_t, smorzamento );
                d_print(predetto);
                printf("main| risultato della correzione\n");
                //d_limite_velocita(&predetto);
            }else{

                //collissione con altri dischi
                Disco* discoUrtato = d_trova_collisione(predetto, dischi,i);
                if ( discoUrtato != NULL ){ //se diverso da NESSUNA_COLLLISIONE
                    predetto = d_correggi_predizione( disco, *discoUrtato, delta_t, smorzamento);
                    double newdist = d_distanza( *discoUrtato, predetto );
                    d_print(predetto);
                    printf("main| CORREZIONE collissione con disco ID: %d pos(%f,%f) / dist %f \n",discoUrtato->id, discoUrtato->pos.x, discoUrtato->pos.y, newdist);
                    //assert( newdist > 2);
                }
            }

            d_limite_velocita(&predetto);
            disco = predetto;
            dischi[i] = predetto;
            n_cycle++;

            d_print(predetto);
            printf("main| CICLO COMPLETATO\n");

            assert( predetto.pos.y > 0.99 );


        }while( c_is_muovibile(disco, contenitore) && d_is_muovibile(disco,dischi,i,delta_t) );

    }

    for ( int j = 0; j<max; j++ ){
        fprintf(fp, "%f,%f;", dischi[j].pos.x,dischi[j].pos.y );
    }



    fclose(fp);

    return 0;
}
