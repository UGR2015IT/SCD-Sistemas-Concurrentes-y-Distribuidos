/******************************************************************************
 *
 * SCD - Sistemas Concurrentes y Distribuidos
 * Grado en Ingeniería Informática
 *
 * 2014 - Ernesto Serrano <erseco@correo.ugr.es>
 * ---------------------------------------------
 *
 *  El problema del productor-consumidor
 *
 ******************************************************************************/

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h> // Funciones para poder llamar sleep()

using namespace std;

// Atributos
const int num_items = 100;  // Valor constante entre 50 y 100
const int tam_vec = 20;     // Valor constante entre 10 y 20 (debe ser menor que num_items)

int buffer[tam_vec];        // Buffer donde almacenamos los datos producidos
int indice = 0;             // Indice del buffer donde se ha almacenado el dato producido

sem_t puede_producir; // Semáforo que controla al productor (se inicializa a 1)
sem_t puede_consumir; // Semáforo que controla al consumidor (se inicializa a 0)
sem_t mutex1;         // Semáforo para que no se solapen los cout (Se inicializa a 1)

// Metodos
int producir_dato()
{
    static int contador = 1;
    return contador++;

}

void consumir_dato(int dato)
{
    // NOTA: Para comprobar como se llena el buffer podemos hacer una pausa de
    // cada vez que se consuma un dato
    //sleep(1);
    cout << "dato recibido: " << dato << endl;
}


void * productor(void *)
{
    for(unsigned i=0;i<num_items;i++)
    {
        // Se debe producir fuera de la exclusion mutua
        int dato = producir_dato();

        sem_wait(&puede_producir);
        sem_wait(&mutex1);

        // falta: insertar ”dato” en el vector
        cout << "dato producido: " << dato << endl;
        buffer[indice] = dato;
        indice++;

        sem_post(&mutex1);
        sem_post(&puede_consumir);
    }
    return NULL;
}

void * consumidor(void *)
{
    for(unsigned i=0;i<num_items;i++)
    {
        sem_wait(&puede_consumir);
        sem_wait(&mutex1);

        int dato;
        // falta: leer ”dato” desde el vector intermedio
        dato = buffer[indice-1];
        indice--;

        sem_post(&mutex1);
        sem_post(&puede_producir);

        // Se debe consumir fuera de la exclusion mutua
        consumir_dato(dato);
    }
    return NULL;

}

// Punto de entrada al programa
int main(int argc, char *argv[])
{
    // Inicializamos los semáforos
    sem_init(&puede_producir, 0, tam_vec); // inicialmente se puede producir (hasta el tam_vec)
    sem_init(&puede_consumir, 0, 0); // inicialmente no se puede consumir
    sem_init(&mutex1, 0, 1); // semaforo para EM: inicializado a 1, se utiliza para el pintado

    // Creamos las hebras
    pthread_t hebra_productor, hebra_consumidor;

    pthread_create(&hebra_productor,NULL,productor,NULL);
    pthread_create(&hebra_consumidor,NULL,consumidor,NULL);


    // permite continuar a hebra_productor y hebra_consumidor
    pthread_join(hebra_productor, NULL);
    pthread_join(hebra_consumidor, NULL);

    // Destruimos los semaforos
    sem_destroy(&puede_producir);
    sem_destroy(&puede_consumir);
    sem_destroy(&mutex1);


    // Fin del programa
    exit(0);

}

