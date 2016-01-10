/*
=================================================
	LA CENA DE FILOSOFOS CON CAMARERO
=================================================


Al final del codigo hay una salida ejemplo.
*/


#include <iostream>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"
#include <mpi.h>

using namespace std;

#define ERROR 0
#define ACK 1
#define PETICION 2
#define COGER 3
#define SOLTAR 4
#define LEVANTARSE 9
#define CAMARERO 10

void Filosofo( int id, int nprocesos);
void Tenedor( int id, int nprocesos);
void Camarero();

// ---------------------------------------------------------------------

int main( int argc, char** argv){
	int rank, size;

	srand(time(0));
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	if( size!=11){
		if( rank == 0) 
			cout<<"El numero de procesos debe ser 11" << endl << flush ;
	} else {
		if (rank == CAMARERO)
			Camarero();		//El proceso 10 es el Camarero
		else if ((rank%2) == 0)  
			Filosofo(rank,size); // Los pares son Filosofos 
		else 
			Tenedor(rank,size);  // Los impares son Tenedores 
	}
	MPI_Finalize( );
	return 0;
}  
// ---------------------------------------------------------------------

void Filosofo(int id, int nprocesos) {
	int izq = (id+1) % nprocesos, der = ((id+nprocesos)-1) % nprocesos;
	MPI_Status status;

	while(1){    
		// Piensa (espera bloqueada aleatorio del proceso)
		cout << "Filosofo " << id << " PENSANDO" << endl << flush;
		// espera bloqueado durante un intervalo de tiempo aleatorio 
		// (entre una décima de segundo y un segundo)
		usleep( 1000U * (100U+(rand()%900U)));

		// Peticion para sentarse a la mesa
		MPI_Ssend(NULL,0,MPI_INT,CAMARERO,PETICION,MPI_COMM_WORLD);
		MPI_Recv(NULL, 0, MPI_INT, CAMARERO, MPI_ANY_TAG ,MPI_COMM_WORLD, &status);
		if (status.MPI_TAG == ACK){
			// Solicita tenedor izquierdo y despues derecho
			MPI_Ssend(NULL,0,MPI_INT,izq,COGER,MPI_COMM_WORLD);
			cout << "Filosofo "<<id<< " coge tenedor izq ..." << izq << endl << flush;
			MPI_Ssend(NULL, 0, MPI_INT, der,COGER, MPI_COMM_WORLD);
			cout <<"Filosofo "<<id<< " coge tenedor der ..." << der << endl << flush;
			// El filosofo ahora puede comer  
			cout<<"Filosofo "<<id<< " COMIENDO"<<endl<<flush;
			sleep((rand() % 3)+1);  //comiendo
			// Suelta el tenedor izquierdo
			MPI_Ssend(NULL,0,MPI_INT,izq,SOLTAR,MPI_COMM_WORLD);
			cout <<"Filosofo "<<id<< " suelta tenedor izq ..." << izq << endl << flush;
			// Suelta el tenedor derecho
			MPI_Ssend(NULL,0,MPI_INT,der,SOLTAR,MPI_COMM_WORLD);
			cout <<"Filosofo "<<id<< " suelta tenedor der ..." << der << endl << flush;
			//Enviar peticion de levantarse
			MPI_Ssend(NULL,0,MPI_INT,CAMARERO,LEVANTARSE,MPI_COMM_WORLD);
			cout <<"Filosofo "<<id<< " se levanta de la mesa." << endl << flush;
		}
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

void Camarero(){
	int mesa = 0, Filo, tag;
	MPI_Status status;
	while(1){
		MPI_Recv(NULL,0,MPI_INT,MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		Filo = status.MPI_SOURCE;
		tag = status.MPI_TAG;
		if (tag == PETICION){
			if (mesa < 4){
				cout << "El camarero proporciona una silla al filosofo "<<Filo<<endl<<flush;
				tag = ACK;
				mesa ++;
			} else {
				cout << "La mesa està LLENA!" << endl << flush;
				tag = ERROR;
			}
			MPI_Ssend(NULL,0,MPI_INT,Filo,tag,MPI_COMM_WORLD);
		} else if (tag == LEVANTARSE) {
			cout << "Una silla està ahora vacia." << endl << flush;
			mesa--;
		}
	}
}
// ---------------------------------------------------------------------

/*
==============================================
		SALIDA EJEMPLO
==============================================

dave95@G6-DVX:~/Scrivania/MPI/Practica$ make filcam
mpicxx -g -Wall  -o filosofos_con_camarero filosofos_con_camarero.cpp
mpirun -np 11 ./filosofos_con_camarero
Filosofo 0 PENSANDO
Filosofo 2 PENSANDO
Filosofo 4 PENSANDO
Filosofo 6 PENSANDO
Filosofo 8 PENSANDO
El camarero proporciona una silla al filosofo 0
El camarero proporciona una silla al filosofo 2
El camarero proporciona una silla al filosofo 4
El camarero proporciona una silla al filosofo 6
La mesa està LLENA!
Filosofo 8 PENSANDO
Filosofo 6 coge tenedor izq ...7
Ten. 7 recibe petic. de 6
Filosofo 4 coge tenedor izq ...5
Ten. 5 recibe petic. de 4
Ten. 3 recibe petic. de 2
Filosofo 2 coge tenedor izq ...3
Filosofo 0 coge tenedor izq ...1
Filosofo 0 coge tenedor der ...10
Filosofo 0 COMIENDO
Ten. 1 recibe petic. de 0
La mesa està LLENA!
Filosofo 8 PENSANDO
La mesa està LLENA!
Filosofo 8 PENSANDO
La mesa està LLENA!
Filosofo 8 PENSANDO
Filosofo 0 suelta tenedor izq ...1
Ten. 1 recibe liberac. de 0
Ten. 1 recibe petic. de 2
Filosofo 0 suelta tenedor der ...10
Filosofo 2 coge tenedor der ...1
Filosofo 2 COMIENDO
Una silla està ahora vacia.
Filosofo 0 se levanta de la mesa.
Filosofo 0 PENSANDO
El camarero proporciona una silla al filosofo 8
Ten. 9 recibe petic. de 8
Filosofo 8 coge tenedor izq ...9
La mesa està LLENA!

*/
