//
// Created by dave95 on 23/10/15.
//

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

const unsigned
    tam_vector = 10,
    max_iter=50,
    num_hebras=10;

int myVector[tam_vector], i = 0, contador = 0, index_prod = 0 , index_cons=0;

sem_t mutex, puede_leer, puede_escribir;

void rand_usleep(){
    unsigned miliseg = 100U + (rand() % 1900U) ;
    usleep( 1000U*miliseg );
}

// ---------------------------------------------------------------------
unsigned producir_dato() {
    rand_usleep();
    cout << "dato producido: " << contador << endl << flush ;
    return contador++ ;
}
// ---------------------------------------------------------------------
void consumir_dato( int dato ) {
    rand_usleep();
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void* producir(void * ){

    int dato = producir_dato() ;

    sem_wait(&puede_escribir);
    sem_wait(&mutex);
    myVector[index_prod] = dato;
    index_prod++; /*% tam_vector*/
    //cout << "Index value : " <<index_prod <<endl;
    sem_post(&mutex);
    sem_post(&puede_leer);

}

void* consumir(void*){
    int dato;

    sem_wait(&puede_leer);
    sem_wait(&mutex);
    dato = myVector[index_cons];
    index_cons++;/*%tam_vector*/
    sem_post(&puede_escribir);
    sem_post(&mutex);

    cout << "index value: " << index_cons <<endl;
    consumir_dato( dato ) ;
}

int main(){

    srand( time(NULL));

    sem_init(&mutex, 0, 1);
    sem_init(&puede_leer,0,0);
    sem_init(&puede_escribir,0,1);

    pthread_t hebra_prod[num_hebras], hebra_cons[num_hebras];

    for (i=0;i<num_hebras;i++){
        pthread_create(&hebra_prod[i], NULL, producir, NULL);
        pthread_create(&hebra_cons[i], NULL, consumir, NULL);
    }

    for (i=0;i<num_hebras;i++){
        pthread_join(hebra_prod[i], NULL);
        pthread_join(hebra_cons[i], NULL);
    }

    pthread_exit(NULL);

    sem_destroy(&puede_leer);
    sem_destroy(&puede_escribir);
    sem_destroy(&mutex);

    return 0;
}
