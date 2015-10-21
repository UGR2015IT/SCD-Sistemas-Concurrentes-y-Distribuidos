/*
 * File:   main.cpp
 * Author: dave95
 *
 * Created on 14 october 2015, 18.55
 */

#include <cstdlib>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

using namespace std;

const int numPersone = 100;
sem_t estanquero;
sem_t fumador[3];
sem_t cerillas, tabaco, papel, objRec;

sem_t tomaAlgo(int);
void* procFumador(sem_t);
void* procEstanq();
void fumar();

int main() {
    srand(time(NULL));
    return 0;
}

void fumar(){
    //  inicializa la semilla aleatoria  (solo la primera vez)
    static bool primera_vez = true ;
    if ( primera_vez )
    {   primera_vez = false ;
        srand( time(NULL) );
    }

    // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
    const unsigned miliseg = 100U + (rand() % 1900U) ;

    // retraso bloqueado durante 'miliseg' milisegundos
    usleep( 1000U*miliseg );
}

sem_t tomaAlgo(int randNumb){
    switch (randNumb)
            case 0:
                return cerillas;
            case 1:
                return tabaco;
            case 2:
                return papel;
}

void* procFumador(sem_t semaforo){ //(*)
    sem_wait(semaforo);
    sem_post(objRec);
    fumar();
    sem_wait();
}

void* procEstanq(){
    int randNumb = rand % 3;
    sem_post(tomaAlgo(randNumb));
    sem_wait(objRec);
}

//ARRAY DI SEMAFORI!!!
//Meglio passare un intero (vedi seminario) che passare un semaforo (*)

// TIPOLOGIA ESAME (PURA PROGRAMMAZIONE)
//Modifica esercizio della pratica
//Esercizio completamente nuova

