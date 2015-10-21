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
sem_t sem_estanquero;
sem_t sem_fumador[3];

sem_t tomaAlgo(int);

void* procFumador(sem_t);
void* procEstanq();
void fumar();
void* fumar(void*);

int main() {

    // Inicializa la semilla aleatoria
    srand( time(NULL) );

    // Inicializamos los semáforos
    sem_init(&mutex1, 0, 1);         // NOTA: ¿Hace falta para el cout realmente?
    sem_init(&sem_estanquero, 0, 1); // inicialmente se puede producir
    sem_init(&sem_fumador[0], 0, 0); // inicialmente no se puede fumar
    sem_init(&sem_fumador[1], 0, 0); // inicialmente no se puede fumar
    sem_init(&sem_fumador[2], 0, 0); // inicialmente no se puede fumar

    // Declaramos las hebras
    pthread_t hebra_estanquero, hebra_fumador1, hebra_fumador2, hebra_fumador3;

    // Inicializamos las hebras
    pthread_create(&hebra_estanquero,NULL,producir,NULL);
    pthread_create(&hebra_fumador1,NULL,fumar,(void *)0);
    pthread_create(&hebra_fumador2,NULL,fumar,(void *)1);
    pthread_create(&hebra_fumador3,NULL,fumar,(void *)2);

    // Lanza las hebras
    pthread_join(hebra_estanquero, NULL);
    pthread_join(hebra_fumador1, NULL);
    pthread_join(hebra_fumador2, NULL);
    pthread_join(hebra_fumador3, NULL);

    // Destruimos los semaforos, no tiene mucho sentido, ya que saldremos con Ctrl+C
    sem_destroy(&sem_estanquero);
    sem_destroy(&sem_fumador[0]);
    sem_destroy(&sem_fumador[1]);
    sem_destroy(&sem_fumador[2]);

    return 0;
}

void * fumar(void * index_fum)
{
    // número o índice de esta hebra
    unsigned long ifum = (unsigned long) index_fum ;

    while (true)
    {
        sem_wait(&sem_fumador[ifum]);

        // Como comenzamos a fumar, le decimos al estanquero que puede repartir
        sem_post(&sem_estanquero);

        sem_wait(&mutex1);
        //cout << pinta_color(ih, "Comienza a fumar el fumador: ")  << ih << endl;
        //pinta_color(ih, "Comienza a fumar el fumador: ");
        cout << "Comienza a fumar el fumador: "<<ifum<<endl;

        sem_post(&mutex1);

        // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
        const unsigned miliseg = 100U + (rand() % 1900U) ;
        usleep( 1000U*miliseg ); // retraso bloqueado durante miliseg milisegundos

        sem_wait(&mutex1);
        // cout << pinta_color(ih, "Termina de fumar el fumador: ") << ih << endl << endl;
        //pinta_color(ih, "Termina de fumar el fumador: ");
        cout << "Comienza a fumar el fumador: "<<ifum<<endl;
        sem_post(&mutex1);

    }

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

