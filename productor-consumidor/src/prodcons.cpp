#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std ;

// ---------------------------------------------------------------------
// constantes 
const unsigned
        num_items  = 40 ,
        tam_vector = 10 ;

// ---------------------------------------------------------------------

unsigned producir_dato()
{
    static int contador = 0 ;
    cout << "producido: " << contador << endl << flush ;
    return contador++ ;
}
// ---------------------------------------------------------------------

void consumir_dato( int dato )
{
    cout << "dato recibido: " << dato << endl ;
}
// ---------------------------------------------------------------------

void * productor( void * )
{
    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        int dato = producir_dato() ;

        // falta: insertar "dato" en el vector
        // ................

    }
    return NULL ;
}
// ---------------------------------------------------------------------

void * consumidor( void * )
{
    for( unsigned i = 0 ; i < num_items ; i++ )
    {
        int dato ;

        // falta: leer "dato" desde el vector intermedio
        // .................

        consumir_dato( dato ) ;
    }
    return NULL ;
}
//----------------------------------------------------------------------

int main()
{

    // falta: crear y poner en marcha las hebras, esperar que terminen
    // ....

    // ==================================
    int my_vector[tam_vector] = {};
// ==================================

    pthread_t hebra_productor[num_items], hebra_consumidor[num_items];

    for (unsigned int i = 0; i < num_items; i++){
        void* arg_ptr = (void*) i;
        pthread_create(&hebra_productor[i], NULL, productor(), NULL);
        pthread_create(&hebra_consumidor[i], NULL, consumidor(), NULL);
    }

    for (unsigned int i=0;i<num_items;i++){
        pthread_join(&hebra_productor[i],NULL);
        pthread_join(&hebra_consumidor[i],NULL);
    }

    pthread_exit (NULL);

    return 0 ;
}