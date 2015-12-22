// *********************************************************************
// SCD (13-14):   Ejemplos del seminario 1  zz
//                Carlos Ureña, Octubre-2013

#include <iostream>
#include <pthread.h>

using namespace std ;


// *********************************************************************
// Ejemplo 1 : creación de dos hebras desde la principal

namespace ejemplo1
{

void* proc1( void* arg )
{  
   for( unsigned long i = 0 ; i < 10 ; i++ )
      cout << "hebra 1, i == " << i << endl ;
   return NULL ;
}
void* proc2( void* arg )
{  
   for( unsigned long i = 0 ; i < 10 ; i++ )
      cout << "hebra 2, i == " << i << endl ;
   return NULL ;
}
void main()
{
   cout << "Ejemplo 1 : creación de dos hebras desde la principal" << endl ;
   
   pthread_t hebra1, hebra2 ;
   pthread_create(&hebra1,NULL,proc1,NULL);
   pthread_create(&hebra2,NULL,proc2,NULL);
   //pthread_exit(NULL); // permite continuar a hebra1 y hebra2
}

} ;

// *********************************************************************
// Ejemplo 2 : creacion y join de dos hebras desde la principal

namespace ejemplo2
{

void* proc1( void* arg )
{  
   for( unsigned long i = 0 ; i < 10 ; i++ )
      cout << "hebra 1, i == " << i << endl ;
   return NULL ;
}
void* proc2( void* arg )
{  
   for( unsigned long i = 0 ; i < 10 ; i++ )
      cout << "hebra 2, i == " << i << endl ;
   return NULL ;
}
   
void main()
{
   cout << "Ejemplo 2: creacion y join de dos hebras desde la principal." << endl ;
   
   pthread_t hebra1, hebra2 ;

   pthread_create(&hebra1,NULL,proc1,NULL);
   pthread_create(&hebra2,NULL,proc2,NULL);

   pthread_join(hebra1,NULL);
   pthread_join(hebra2,NULL);
   cout << "hebras terminadas." << endl ;
   // calculo posterior a la finalizacion de las hebras.....
}

};

// *********************************************************************
// Ejemplo 3 : creación y join de N hebras idénticas usando vectores de 
//             identificadores de hebras.

namespace ejemplo3
{
   
const unsigned num_hebras = 10 ; 

unsigned long func( unsigned long n ) 
{ /* ..calculo arbitrario.. */ 
   return n*(n-1)/2 ;
} 
void* proc( void* i ) 
{ 
   return (void *) func( (unsigned long)(i) ) ; 
}

void main()
{
   cout << "Ejemplo 3: creación y join de N hebras idénticas usando vectores de identificadores de hebras" << endl ;
   
   pthread_t id_hebra[num_hebras] ;
   for( unsigned long i = 0 ; i < num_hebras ; i++ )
       pthread_create( &(id_hebra[i]), NULL, proc, (void *)i );

   for( unsigned i = 0 ; i < num_hebras ; i++ )
   {   unsigned long resultado ;
       pthread_join( id_hebra[i], (void **) (&resultado) );
       cout << "func(" <<  i << ") == " << resultado << endl  ;
   }
}
   
} ;

// *********************************************************************
// Ejemplo 4 : código y plantilla para el cálculo de PI

namespace ejemplo4
{


const unsigned long m = long(1024)*long(1024) ;  // número de muestras (un millón)
const unsigned n      = 4 ;                      // número de hebras

double resultado_parcial[n] ; // tabla de sumas parciales (una por hebra)

double f( double x )       // implementa función $f$
{ 
   return 4.0/(1+x*x) ;     // $~~~~f(x)\,=\,4/(1+x^2)$
}

double calcular_integral_secuencial( )
{  
   double suma = 0.0 ;                      // inicializar suma
   for( unsigned long i = 0 ; i < m ; i++ ) // para cada $i$ entre $0$ y $m-1$
      suma += f( (i+0.5)/m );               // $~~~~~$ añadir $f(x_i)$ a la suma actual
   return suma/m ;                          // devolver valor promedio de $f$
}


void * funcion_hebra( void * ih_void ) // función que ejecuta cada hebra
{  
   unsigned long ih = (unsigned long) ih_void ; // número o índice de esta hebra
   double sumap = 0.0 ;
   // calcular suma parcial en "sumap"
   //.....
   resultado_parcial[ih] = sumap ; // guardar suma parcial en vector.
   return NULL ;
}

double calcular_integral_concurrente( )
{  
   // crear y lanzar $n$ hebras, cada una ejecuta "funcion\_concurrente"
   //.....
   // esperar (join) a que termine cada hebra, sumar su resultado
   //.....
   // devolver resultado completo
   // .....
   return 0.0 ; // (cambiar)
}

void main()
{
   
   cout << "Ejemplo 4 (cálculo de PI)" << endl ;
   
   cout << "valor de PI (calculado secuencialmente) == " << calcular_integral_secuencial() << endl 
        << "valor de PI (calculado concurrentemente) == " << calcular_integral_concurrente() << endl ; 
    
}

} ; 

//**********************************************************************

int main( int argc, char * argv[] )
{
   ejemplo1::main() ;
   ejemplo2::main() ;
   ejemplo3::main() ;
   ejemplo4::main() ;
   pthread_exit(NULL);
}
