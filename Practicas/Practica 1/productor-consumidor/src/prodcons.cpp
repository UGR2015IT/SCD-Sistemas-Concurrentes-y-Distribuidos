#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned
        num_items  = 40 ,
        tam_vector = 10 ;

int index_producir = 0;
int index_consumir = 0;

int myVector[tam_vector]={};

sem_t mutex, puede_leer, puede_escribir;

// ---------------------------------------------------------------------
unsigned producir_dato() {
    static int contador = 0 ;
    sleep(0.5);
    cout << "dato producido: " << contador << endl << flush ;
    return contador++ ;
}
// ---------------------------------------------------------------------
void consumir_dato( int dato ) {
    sleep(0.8);
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------
void * productor( void * p) {
    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        int dato = producir_dato() ;

        // falta: insertar "dato" en el vector
        // ................

        sem_wait(&puede_escribir);
        sem_wait(&mutex);
        myVector[index_producir]=dato;
        index_producir = (index_producir+1) % tam_vector;
        sem_post(&mutex);
        sem_post(&puede_leer);

    }
    return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * p) {
    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        int dato;

        // ===================================
        // Added by me
        // ===================================

        sem_wait(&puede_leer);
        sem_wait(&mutex);
        dato = myVector[index_consumir];
        index_consumir = (index_consumir+1)%tam_vector;
        sem_post(&puede_escribir);
        sem_post(&mutex);

        consumir_dato( dato ) ;
    }
    return NULL ;
}
//----------------------------------------------------------------------

int main()
{

    // falta: crear y poner en marcha las hebras, esperar que terminen
    // ....


    sem_init( &mutex,          0, 1 ); // semaforo para EM: inicializado a 1
    sem_init( &puede_escribir, 0, num_items ); // inicialmente se puede escribir
    sem_init( &puede_leer,     0, 0 ); // inicialmente no se puede leer

    pthread_t hebra_productor, hebra_consumidor;

    pthread_create(&hebra_productor, NULL, productor, NULL);
    pthread_create(&hebra_consumidor, NULL, consumidor, NULL);

    pthread_join(hebra_consumidor, NULL);
    pthread_join(hebra_productor, NULL);

    sem_destroy(&mutex);
    sem_destroy(&puede_escribir);
    sem_destroy(&puede_leer);

    return 0 ;
}