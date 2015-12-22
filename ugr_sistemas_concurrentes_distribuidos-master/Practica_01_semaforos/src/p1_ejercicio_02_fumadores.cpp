/******************************************************************************
 *
 * SCD - Sistemas Concurrentes y Distribuidos
 * Grado en Ingeniería Informática
 *
 * 2014 - Ernesto Serrano <erseco@correo.ugr.es>
 * ---------------------------------------------
 *
 *  El problema de los fumadores.
 *
 ******************************************************************************/
#include <cstdlib>
#include <iostream>
#include <string>
#include <iomanip>
#include <pthread.h>
#include <semaphore.h>
#include <time.h> // incluye ”time(....)”
#include <unistd.h> // incluye ”usleep(...)”
#include <stdlib.h> // incluye ”rand(...)” y ”srand”

using namespace std;

// Atributos
sem_t sem_estanquero;   // Semáforo  del estanquero (inicializado a 1)
sem_t sem_fumador[3];   // Array de 3 semaforos para los fumadores (inicializados a 0)
sem_t mutex1;           // Semáforo para que no se solapen los cout (Se inicializa a 1)

// Hace un cout con colorines para analizar mas facilmente los resultados
void pinta_color(int fumador, string cadena)
{
    switch (fumador)
    {
        case 0:
            // Rojo
            cout << "\033[1;31m" << cadena << " " << fumador << "\033[0m" << endl;
            break;
        case 1:
            // Azul
            cout << "\033[1;34m" << cadena << " " << fumador << "\033[0m" << endl;
            break;
        case 2:
            // Verde
            cout << "\033[1;32m" << cadena << " " << fumador << "\033[0m" << endl;
            break;
    }

}

// funcion que simula la accion de fumar
// como un retardo aleatorio de la hebra
void * fumar(void * ih_void)
{
    // número o índice de esta hebra
    unsigned long ih = (unsigned long) ih_void ;

    while (true)
    {
        // sem_wait(&mutex1);
        // cout << "Fumador: " << ih << " esperando a fumar, espera ing: " << ih << endl;
        // sem_post(&mutex1);

        sem_wait(&sem_fumador[ih]);

        // Como comenzamos a fumar, le decimos al estanquero que puede repartir
        sem_post(&sem_estanquero);

        sem_wait(&mutex1);
        //cout << pinta_color(ih, "Comienza a fumar el fumador: ")  << ih << endl;
        pinta_color(ih, "Comienza a fumar el fumador: ");

        sem_post(&mutex1);

        // calcular un numero aleatorio de milisegundos (entre 1/10 y 2 segundos)
        const unsigned miliseg = 100U + (rand() % 1900U) ;
        usleep( 1000U*miliseg ); // retraso bloqueado durante miliseg milisegundos

        sem_wait(&mutex1);
        // cout << pinta_color(ih, "Termina de fumar el fumador: ") << ih << endl << endl;
        pinta_color(ih, "Termina de fumar el fumador: ");
        sem_post(&mutex1);

    }

}

void * producir(void *)
{
    while (true)
    {
        sem_wait(&sem_estanquero);

        // calcular un numero aleatorio entre 0 y 2
        unsigned int ingrediente = (rand() % 3U) ;

        sem_wait(&mutex1);
        cout << "Producido el ingrediente: " << ingrediente << endl;
        sem_post(&mutex1);

        sem_post(&sem_fumador[ingrediente]);
    }
}

// Punto de entrada al programa
int main(int argc, char *argv[])
{
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

    // Fin del programa, como usamos bucle infinito nunca pasará por aqui
    exit(0);

}

