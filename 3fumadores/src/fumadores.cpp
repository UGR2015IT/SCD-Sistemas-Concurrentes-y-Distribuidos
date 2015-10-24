#include <iostream>
#include <cassert>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>      // incluye "time(....)"
#include <unistd.h>    // incluye "usleep(...)"
#include <stdlib.h>    // incluye "rand(...)" y "srand"

using namespace std;

// ============================================================================
// Constant sections

const static int n_fumadores = 3,
                    n_hebras = 10;

sem_t sem_fumador[3], sem_estanquero;

// ============================================================================
// función que simula la acción de fumar  como un retardo aleatorio de la hebra

void fumar() {
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
// ============================================================================

// Function section

void* fumadores(void* n_fum){
    while (true){
        sem_wait(&sem_fumador[(unsigned long)n_fum]);
        sem_post(&sem_estanquero);
        cout << "The smoker n°"<<((unsigned long)n_fum )+1<< " is smokin'. They hatin'."<<endl;
        fumar();
        cout << "The smoker n°"<<((unsigned long)n_fum )+1<< " is done."<<endl;
    }
}

void* estanquero(void*p){
    while (true){
        sem_wait(&sem_estanquero);
        unsigned int index_fumador = (rand() % n_fumadores);
        //cout << "The smoker n°"<<index_fumador+1<<" is gonna be smokin' soon."<<endl;
        sem_post(&sem_fumador[index_fumador]);
    }

}

// ============================================================================

int main()
{

    for (int i=0; i<n_fumadores;i++){
        sem_init(&sem_fumador[i] , 0 , 0);
    }
    sem_init(&sem_estanquero, 0, 1);

    pthread_t hebra_fumador[n_fumadores], hebra_estanquero;

    cout << "Initialized semaphores & hebras" << endl;

    for (int i=0;i<n_fumadores;i++){
        cout << "Created the smoker n°"<<i+1<<endl;
        pthread_create(&(hebra_fumador[i]), NULL, fumadores, (void*) i);
    }
    pthread_create (&hebra_estanquero, NULL, estanquero, NULL);

    cout << "Created the hebras and executed them" <<endl;

    for (int i=0;i<n_fumadores;i++)
        pthread_join(hebra_fumador[i], NULL);
    pthread_join(hebra_estanquero, NULL);

    cout << "Joined the hebras, almost done."<<endl;

    pthread_exit(NULL);

    for (int i=0;i<n_fumadores;i++)
        sem_destroy(&sem_fumador[i]);
    sem_destroy(&sem_estanquero);
    
    return 0 ;
}
