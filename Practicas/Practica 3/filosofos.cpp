/*
====================================================
				LA CENA DE FILOSOFOS
====================================================
En la cena de filosofos, cada filosofo tiene que coger dos tenedores para comer, y soltar los una vez que ha comido. Para hacer esto con MPI, necesitamos que cada filosofo y cada tenedor tenga su proceso asociado, y que se envien peticiones de COGER (MPI_TAG=1) y de SOLTAR (MPI_TAG=1). Cada tenedor, una vez cogido por un filosofo (MPI_SOURCE de la peticion enviada antes), espera que el mismo filosofo lo deje.
El problema de la cena de los filosofos puede conducir a una situacion de interbloqueo si todos los filosofos cogen el tenedor izquierdo al mismo tiempo, y entonces esperan todos la suelta del tenedor derecho que pero nunca se va a soltar. Para solucionar este problema, el filosofo 0 y los filosofos pares (es decir, los filosofos con proceso multiplo de 4) cogen primero el tenedor derecho, mientras los otros cogen primero el tenedor izquierdo.

Al final del codigo hay una salida ejemplo.
*/


#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

#define SOLTAR 0
#define COGER 1

void Filosofo( int id, int nprocesos);
void Tenedor ( int id, int nprocesos);

// ---------------------------------------------------------------------

int main( int argc, char** argv )
{
   int rank, size;
   
   srand(time(0));
   MPI_Init( &argc, &argv );
   MPI_Comm_rank( MPI_COMM_WORLD, &rank );
   MPI_Comm_size( MPI_COMM_WORLD, &size );
   
   if( size!=10)
   {
      if( rank == 0) 
         cout<<"El numero de procesos debe ser 10" << endl << flush ;
      MPI_Finalize( ); 
      return 0; 
   }
   
   if ((rank%2) == 0)  
      Filosofo(rank,size); // Los pares son Filosofos 
   else 
      Tenedor(rank,size);  // Los impares son Tenedores 
   
   MPI_Finalize( );
   return 0;
}  
// ---------------------------------------------------------------------

void Filosofo(int id, int nprocesos) {
	int izq = (id+1) % nprocesos;
	int der = ((id+nprocesos)-1) % nprocesos;

	while(1){
		if (id%4==0){
			// Solicita tenedor izquierdo y despues derecho
			MPI_Ssend(NULL,0,MPI_INT,izq,COGER,MPI_COMM_WORLD);
			cout << "Filosofo "<<id<< " solicita tenedor izq ..." << izq << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, der,COGER, MPI_COMM_WORLD);
			cout <<"Filosofo "<<id<< " coge tenedor der ..." << der << endl << flush;
		} else {
			// Solicita tenedor derecho y despues izquierdo
			MPI_Ssend(NULL,0,MPI_INT,der,COGER,MPI_COMM_WORLD);
			cout << "Filosofo "<<id<< " solicita tenedor der ..." << izq << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, izq,COGER, MPI_COMM_WORLD);
			cout <<"Filosofo "<<id<< " coge tenedor izq ..." << der << endl << flush;
		}
		// El filosofo ahora puede comer  
		cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
		sleep((rand() % 3)+1);  //comiendo
		// Suelta el tenedor izquierdo
		MPI_Ssend(NULL,0,MPI_INT,izq,SOLTAR,MPI_COMM_WORLD);
		cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
		MPI_Ssend(NULL,0,MPI_INT,der,SOLTAR,MPI_COMM_WORLD);
		// Suelta el tenedor derecho
		cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;      
		// Piensa (espera bloqueada aleatorio del proceso)
		cout << "Filosofo " << id << " PENSANDO" << endl << flush;
		// espera bloqueado durante un intervalo de tiempo aleatorio 
		// (entre una décima de segundo y un segundo)
		usleep( 1000U * (100U+(rand()%900U)) );
	}
}
// ---------------------------------------------------------------------

void Tenedor(int id, int nprocesos) {
	int Filo; 
	MPI_Status status;
  
	while(1) {
		// Espera un peticion desde cualquier filosofo vecino ...
		MPI_Probe(MPI_ANY_SOURCE,COGER,MPI_COMM_WORLD, &status);
		Filo = status.MPI_SOURCE;
		MPI_Recv(NULL,0,MPI_INT,MPI_ANY_SOURCE,COGER, MPI_COMM_WORLD,&status);
		cout << "Ten. " << id << " recibe petic. de " << Filo << endl << flush;
		// Espera a que el filosofo suelte el tenedor...
		MPI_Probe(Filo,SOLTAR,MPI_COMM_WORLD, &status);
		MPI_Recv(NULL,0,MPI_INT,Filo,SOLTAR,MPI_COMM_WORLD,&status);
		cout << "Ten. " << id << " recibe liberac. de " << Filo << endl << flush;
	}
}
// ---------------------------------------------------------------------

/*
==============================================
				SALIDA EJEMPLO
==============================================

dave95@G6-DVX:~/Scrivania/MPI/Practica$ make f
mpicxx -g -Wall  -o filosofos filosofos.cpp
mpirun -np 10 ./filosofos
Ten. 1 recibe petic. de 0
Ten. 9 recibe petic. de 8
Filosofo 0 solicita tenedor izq ...1
Filosofo 8 solicita tenedor izq ...9
Ten. 5 recibe petic. de 4
Filosofo 4 solicita tenedor izq ...5
Filosofo 4 coge tenedor der ...3
Filosofo 4 COMIENDO
Ten. 3 recibe petic. de 4
Filosofo 8 coge tenedor der ...7
Ten. 7 recibe petic. de 8
Filosofo 8 COMIENDO
Filosofo 4 suelta tenedor izq ...5
Filosofo 4 suelta tenedor der ...3
Filosofo 4 PENSANDO
Ten. 5 recibe liberac. de 4
Ten. 5 recibe petic. de 6
Ten. 3 recibe liberac. de 4
Filosofo 6 solicita tenedor der ...7
Ten. 7 recibe liberac. de 8
Ten. 7 recibe petic. de 6
Filosofo 6 coge tenedor izq ...5
Filosofo 6 COMIENDO
Ten. 9 recibe liberac. de 8
Ten. 9 recibe petic. de 0
Filosofo 8 suelta tenedor izq ...9
Filosofo 8 suelta tenedor der ...7
Filosofo 8 PENSANDO
Filosofo 0 coge tenedor der ...9
Filosofo 0 COMIENDO
Filosofo 6 suelta tenedor izq ...7
Ten. 7 recibe liberac. de 6
Ten. 1 recibe liberac. de 0
Ten. 1 recibe petic. de 2
Filosofo 0 suelta tenedor izq ...1
Ten. 9 recibe liberac. de 0
Ten. 9 recibe petic. de 8
Filosofo 8 solicita tenedor izq ...9
Filosofo 8 coge tenedor der ...7
Filosofo 8 COMIENDO
Ten. 7 recibe petic. de 8
Filosofo 0 suelta tenedor der ...9
Filosofo 0 PENSANDO
Filosofo 2 solicita tenedor der ...3
Ten. 3 recibe petic. de 2
*/
