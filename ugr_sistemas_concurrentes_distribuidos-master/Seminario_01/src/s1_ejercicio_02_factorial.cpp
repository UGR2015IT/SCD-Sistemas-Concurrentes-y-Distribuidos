/******************************************************************************
 *
 * SCD - Sistemas Concurrentes y Distribuidos
 * Grado en Ingeniería Informática
 *
 * 2014 - Ernesto Serrano <erseco@correo.ugr.es>
 * ---------------------------------------------
 *
 *  Ejemplo de calculo de los decimales de PI de forma secuencial y concurrente
 *
 ******************************************************************************/
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <pthread.h>
#include "fun_tiempo.h" // Funciones de calculo de tiempo
#include <math.h> // Incluimos las librerías matemáticas para hacer uso de la constante PI

using namespace std;


// Atributos
unsigned long num_muestras;         // numero de muestras (lo pasaremos como parametro al programa)
const unsigned num_hebras  = 4 ;    // número de hebras (lo ponemos fijo)

double resultado_parcial[num_hebras] ; // tabla de sumas parciales (una por hebra)


double f( double x ) // implementa funcion f
{
    return 4.0/(1+x*x) ; // f(x) = 4/(1+x2)
}


double calcular_integral_secuencial( )
{

    double suma = 0.0 ; // inicializar suma

    for ( unsigned long i = 0 ; i < num_muestras ; i++ ) //para cada i entre 0 y num_muestras−1
        suma += f( (i+0.5)/num_muestras );              // añadir f(xi) a la suma actual

    return suma/num_muestras ; // devolver valor promedio de f

}

void * funcion_hebra( void * ih_void ) // función que ejecuta cada hebra
{
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ;
   // calcular suma parcial en "sumap"
    for ( unsigned long i = ih ; i < num_muestras ; i+=num_hebras ) //para cada i entre ih y num_muestras−1 pero de 4 en 4 (numero de hebras)
        sumap += f( (i+0.5)/num_muestras );             // añadir f(xi) a la suma actual


   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   return NULL ;
}

double calcular_integral_concurrente( )
{
   // crear y lanzar $n$ hebras, cada una ejecuta "funcion_hebra" pasandole el indice (i)
    pthread_t id_hebra[num_hebras] ;
    for( unsigned long i = 0 ; i < num_hebras ; i++ )
        pthread_create( &(id_hebra[i]), NULL, funcion_hebra, (void *)i );

   // esperar (join) a que termine cada hebra
   for(unsigned i = 0 ; i < num_hebras ; i++ )
   {
        pthread_join( id_hebra[i], NULL);
   }

    // devolver resultado completo (sumamos los resultados parciales)
    double suma = 0.0 ; // inicializamos variable suma

    for (unsigned long i = 0 ; i < num_hebras ; i++ )
        suma += resultado_parcial[i];

    return suma/num_muestras ; // Devolvemos el promedio


}

int main(int argc, char *argv[]) {

    // Comprobamos que se le pasen los parametros correctos al programa
    if (argc != 2)
    {
        cout << "Uso del programa: " << argv[0] << " <numero de muestras>" << endl << endl;
        exit(2); // Salimos con error 2
    }

    // Asignamos el numero de muestras
    num_muestras = atoi(argv[1]);

    struct timespec inicio_s = ahora(); // inicio = inicio del tiempo a medir

    // Hacemos los calculos
    double pi_secuencial = calcular_integral_secuencial();

    struct timespec fin_s = ahora(); // fin = fin del tiempo a medir


    struct timespec inicio_c = ahora(); // inicio = inicio del tiempo a medir

    // Hacemos los calculos
    double pi_concurrente = calcular_integral_concurrente();

    struct timespec fin_c = ahora(); // fin = fin del tiempo a medir


    cout << "Ejemplo 2 (cálculo de PI)" << endl ;

    // Mostramos los valores obtenidos
    cout << setprecision(50) << "Valor de PI: (constante math.h)          == " << M_PI << endl;
    cout << setprecision(50) << "valor de PI (calculado secuencialmente)  == " << pi_secuencial  << " -- (" << setprecision(5) <<  duracion( &inicio_s, &fin_s) << " seg.)" << endl;
    cout << setprecision(50) << "valor de PI (calculado concurrentemente) == " << pi_concurrente << " -- (" << setprecision(5) <<  duracion( &inicio_c, &fin_c) << " seg.)" << endl << endl;


    exit(0);

}

