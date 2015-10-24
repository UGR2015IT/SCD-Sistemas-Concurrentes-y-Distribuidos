// *********************************************************************
// SCD. Ejemplos del seminario 1.
//
// Plantilla para el ejercicio de cálculo de PI
// *********************************************************************

#include <iostream>
#include <pthread.h>

using namespace std ;

// ---------------------------------------------------------------------
// constante y variables globales (compartidas entre hebras)

const unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
const unsigned n      = 4 ;                      // número de hebras

double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)
double new_parcial[n+1];

int seq_operation_counter = 0, conc_operation_counter = 0;

// ---------------------------------------------------------------------
// implementa función $f$

double f( double x )       
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}
// ---------------------------------------------------------------------
// cálculo secuencial

double calcular_integral_secuencial( ){
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ){// para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
      seq_operation_counter++;
   }

   return suma/m ;                          // devolver valor promedio de $f$
}
// ---------------------------------------------------------------------
// función que ejecuta cada hebra

void * funcion_hebra( void * ih_void ) {
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra

   double sumap = 0.0 ;
   // calcular suma parcial en "sumap"
   //.....
   for( unsigned long ih_index = ih*(m/n) ; ih_index < ((ih+1)*m/n) ; ih_index++ ){ // para cada $i$ entre $0$ y $249$
      sumap += f((ih_index+0.5)/m );
      conc_operation_counter++;
   }

   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   return NULL ;
}

void * new_hebra(void* ih_void){
   unsigned long ih = (unsigned long) ih_void;
   double sumap = 0.0;

   for (unsigned long ih_index = ih*(m/(n+1)); ih_index < ((ih+1)*m/(n+1)); ih_index++){
      sumap += f((ih_index+0.5)/m );
      conc_operation_counter++;
   }
   new_parcial[ih] = sumap;
   return NULL;
}
// ---------------------------------------------------------------------
// cálculo concurrente

double calcular_integral_concurrente( ) {
   // crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
   //.....
   pthread_t hebra_calculo[n];
   double suma_conc = 0.0;
   for (int i=0;i<n;i++){
      pthread_create(&hebra_calculo[i], NULL, funcion_hebra, (void*)i);
   }
   for (int i = 0; i < n; i++) {
      pthread_join(hebra_calculo[i], NULL);
      suma_conc += resultado_parcial[i];
      conc_operation_counter++;
   }

   suma_conc = suma_conc/m;
   return suma_conc;
}

double new_concurrent_integral(){
   pthread_t hebra_calculo[n+1];
   double suma_conc_2 = 0.0;
   for (int i=0;i<n+1;i++){
      pthread_create(&hebra_calculo[i], NULL, new_hebra, (void*)i);
   }
   for (int i = 0; i < n+1; i++) {
      pthread_join(hebra_calculo[i], NULL);
      suma_conc_2 += new_parcial[i];
      conc_operation_counter++;
   }

   suma_conc_2 = suma_conc_2/m;
   return suma_conc_2;
}

// ---------------------------------------------------------------------

int main() {

   cout << "Ejemplo 4 (cálculo de PI)" << endl;
   double pi_sec = 0.0, pi_conc = 0.0, pi_conc2 = 0.0;

   pi_sec = calcular_integral_secuencial();

   pi_conc = calcular_integral_concurrente();

   cout << "valor de PI (calculado secuencialmente)  == " << pi_sec << endl
   << "con " << seq_operation_counter << " calcoli sequenziali eseguiti" << endl;
   cout << "valor de PI (calculado concurrentemente, 4 hebras) == " << pi_conc << endl
   << "con " << conc_operation_counter << " calcoli paralleli eseguiti" << endl;

   conc_operation_counter = 0.0;

   pi_conc2 = new_concurrent_integral();

   cout << "New PI valor == " << pi_conc2 << endl
   << "con " << conc_operation_counter << " calcoli paralleli eseguiti" << endl;

   return 0;
}