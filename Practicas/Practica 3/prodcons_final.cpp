/*
================================
	PRODUCTOR CONSUMIDOR MPI
================================
Hay muchos cambios realizados sobre el programa inicial.

EN GENERAL:
- los DEFINE de Productor, Buffer y Consumidor, ponendo como valor el rank de el ultimo proceso (productor), del solo proceso (buffer) y del primero proceso (consumidor); este valor se va a utilisar tambien como TAG nel paso de mensajes entre procesos P/C y buffer.
EN EL PRODUCTOR:
- el bucle tiene como valor maximo el numero de iteracciones partido por el numero de procesos productores (de 0 a 4 son 5 procesos)
- para una mejor impresion en pantalla, se ha añadido el rank del proceso que produce el valor
- la funcion de envio tiene como TAG Productor. Este es necesario para que el buffer entenda de quien llega el mensaje recibido: el tag se usa como un identificador de grupo
EN EL CONSUMIDOR:
- el bucle tiene como valor maximo el numero de iteracciones partido por el numero de procesos productores (de 6 a 9 son 4 procesos)
- para una mejor impresion en pantalla, se ha añadido el rank del proceso que consume el valor
- la funcion de envio tiene como TAG Consumidor, por el mismo razonamiento del productor
EN EL BUFFER:
- si el buffer no esta lleno o vacio, el buffer chequea el TAG del mensaje llegado (sin haberlo recibido) y en funcion de esto elige la rama
- la rama 0 (productor) recibe cualquier mensaje de tag Productor de cualquier fuente (es decir, solo recibe mensajes de productores); status.MPI_SOURCE nos permite de saber de quien ha llegado el mensaje
- la rama 1 (consumidor) hace un paso sincrono de mensaje: recibe cualquier mensaje de tag Consumidor de cualquier fuente (es decir, solo recibe mensajes de consumidor) y envia al mismo (status.MPI_SOURCE) el dato para consumir.

Al final del codigo hay una salida ejemplo del programa.
*/


#include <mpi.h>
#include <iostream>
#include <math.h>
#include <time.h>      // incluye "time"
#include <unistd.h>    // incluye "usleep"
#include <stdlib.h>    // incluye "rand" y "srand"


#define Productor    4	//ultimo rank por procesos Productor
#define Buffer       5
#define Consumidor   6	//primero rank por procesos Consumidor
#define ITERS       20
#define TAM          5

using namespace std;

// ---------------------------------------------------------------------

void productor(int rank){
	int value ;
	for ( unsigned int i=0; i < ITERS/5 ; i++ ){ 
		value = i;
		cout << "Productor "<<rank<<" produce valor " << value << endl << flush ;

		// espera bloqueado durante un intervalo de tiempo aleatorio 
		// (entre una décima de segundo y un segundo)
		usleep( 1000U * (100U+(rand()%900U)) );
		// enviar 'value'
		MPI_Ssend( &value, 1, MPI_INT, Buffer, Productor, MPI_COMM_WORLD );
	}
}
// ---------------------------------------------------------------------

void buffer()
{
	int value[TAM], 
		peticion, 
		pos=0,
		rama;
	MPI_Status status;

	for( unsigned int i=0 ; i < ITERS*2 ; i++ ){  
		if ( pos==0 )      // el consumidor no puede consumir
			rama = 0 ;        
		else if (pos==TAM) // el productor no puede producir
			rama = 1 ;
		else {             // ambas guardas son ciertas
			// leer 'status' del siguiente mensaje (esperando si no hay)
			MPI_Probe( MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status );
			// calcular la rama en función del origen del mensaje
			if ( status.MPI_TAG == Productor ) 
				rama = 0 ; 
			else 
				rama = 1 ;
		}
		switch(rama){
			case 0:
				MPI_Recv( &value[pos], 1, MPI_INT, MPI_ANY_SOURCE, Productor, MPI_COMM_WORLD, &status );
				cout << ">>> Buffer recibe " << value[pos] << " de Productor " << status.MPI_SOURCE << endl << flush;  
				pos++;
				break;
			case 1:
				MPI_Recv( &peticion, 1, MPI_INT, MPI_ANY_SOURCE, Consumidor, MPI_COMM_WORLD, &status );
				MPI_Ssend( &value[pos-1], 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
				cout << ">>> Buffer envía " << value[pos-1] << " de posicion "<<pos-1<<" a Consumidor "<< status.MPI_SOURCE << endl << flush;  
				pos--;
				break;
		}
	}
}   
   
// ---------------------------------------------------------------------

void consumidor(int rank)
{
	int         value,
		       peticion = 1 ; 
	float       raiz ;
	MPI_Status  status ;

	for (unsigned int i=0;i<ITERS/4;i++){
		MPI_Ssend( &peticion, 1, MPI_INT, Buffer, Consumidor, MPI_COMM_WORLD ); 
		MPI_Recv ( &value, 1,    MPI_INT, Buffer, 0, MPI_COMM_WORLD,&status );
		cout << "> Consumidor "<<rank<<" recibe valor " << value << " de Buffer " << endl << flush ;

		// espera bloqueado durante un intervalo de tiempo aleatorio 
		// (entre una décima de segundo y un segundo)
		usleep( 1000U * (100U+(rand()%900U)) );

		raiz = sqrt(value) ;
	}
}
// ---------------------------------------------------------------------

int main(int argc, char *argv[]) 
{
	int rank,size; 

	// inicializar MPI, leer identif. de proceso y número de procesos
	MPI_Init( &argc, &argv );
	MPI_Comm_rank( MPI_COMM_WORLD, &rank );
	MPI_Comm_size( MPI_COMM_WORLD, &size );

	// inicializa la semilla aleatoria:
	srand ( time(NULL) );

	// comprobar el número de procesos con el que el programa 
	// ha sido puesto en marcha (debe ser 10)
	if ( size != 10 ) 
	{
		cout<< "El numero de procesos debe ser 10 "<<endl;
		return 0;
	} 
   
	// verificar el identificador de proceso (rank), y ejecutar la
	// operación apropiada a dicho identificador
	if ( rank <= Productor ) 
		productor(rank);
	else if ( rank == Buffer )
		buffer();
	else 
    	consumidor(rank);
   
	// al terminar el proceso, finalizar MPI
	MPI_Finalize();
	return 0;
}

/*
EJEMPLO DE SALIDA:

dave95@G6-DVX:~/Scrivania/MPI/Practica$ mpirun -np 10 prodconsProductor 0 produce valor 0
Productor 1 produce valor 0
Productor 2 produce valor 0
Productor 3 produce valor 0
Productor 4 produce valor 0
Productor 0 produce valor 1
>>> Buffer recibe 0 de Productor 0
>>> Buffer envía 0 de posicion 0 a Consumidor 6
> Consumidor 6 recibe valor 0 de Buffer 
>>> Buffer recibe 0 de Productor 3
>>> Buffer envía 0 de posicion 0 a Consumidor 7
>>> Buffer recibe 0 de Productor 2
>>> Buffer envía 0 de posicion 0 a Consumidor 8
> Consumidor 7 recibe valor 0 de Buffer 
Productor 2 produce valor 1
Productor 3 produce valor 1
> Consumidor 8 recibe valor 0 de Buffer 
>>> Buffer recibe 0 de Productor 1
Productor 1 produce valor 1
>>> Buffer envía 0 de posicion 0 a Consumidor 9
>>> Buffer recibe 0 de Productor 4
Productor 4 produce valor 1
> Consumidor 9 recibe valor 0 de Buffer 
>>> Buffer envía 0 de posicion 0 a Consumidor 6
> Consumidor 6 recibe valor 0 de Buffer 
Productor 0 produce valor 2
>>> Buffer recibe 1 de Productor 0
>>> Buffer envía 1 de posicion 0 a Consumidor 7
> Consumidor 7 recibe valor 1 de Buffer 
>>> Buffer recibe 1 de Productor 3
Productor 3 produce valor 2
> Consumidor 8 recibe valor 1 de Buffer 
>>> Buffer envía 1 de posicion 0 a Consumidor 8
>>> Buffer recibe 1 de Productor 1
>>> Buffer recibe 1 de Productor 2
>>> Buffer recibe 1 de Productor 4
Productor 1 produce valor 2
>>> Buffer envía 1 de posicion 2 a Consumidor 9
> Consumidor 9 recibe valor 1 de Buffer 
Productor 2 produce valor 2
Productor 4 produce valor 2
>>> Buffer recibe 2 de Productor 0
Productor 0 produce valor 3
>>> Buffer recibe 2 de Productor 3
Productor 3 produce valor 3
Productor 1 produce valor 3
>>> Buffer recibe 2 de Productor 1
> Consumidor 6 recibe valor 2 de Buffer 
>>> Buffer envía 2 de posicion 4 a Consumidor 6
>>> Buffer recibe 2 de Productor 2
Productor 2 produce valor 3
> Consumidor 7 recibe valor 2 de Buffer 
>>> Buffer envía 2 de posicion 4 a Consumidor 7
>>> Buffer recibe 2 de Productor 4
Productor 4 produce valor 3
>>> Buffer envía 2 de posicion 4 a Consumidor 6
> Consumidor 6 recibe valor 2 de Buffer 
> Consumidor 9 recibe valor 2 de Buffer 
>>> Buffer envía 2 de posicion 3 a Consumidor 9
> Consumidor 8 recibe valor 2 de Buffer 
>>> Buffer envía 2 de posicion 2 a Consumidor 8
>>> Buffer recibe 3 de Productor 0
>>> Buffer recibe 3 de Productor 3
>>> Buffer recibe 3 de Productor 1
> Consumidor 7 recibe valor 3 de Buffer 
>>> Buffer envía 3 de posicion 4 a Consumidor 7
>>> Buffer envía 3 de posicion 3 a Consumidor 9
> Consumidor 9 recibe valor 3 de Buffer 
>>> Buffer envía 3 de posicion 2 a Consumidor 8
> Consumidor 8 recibe valor 3 de Buffer 
>>> Buffer recibe 3 de Productor 2
>>> Buffer recibe 3 de Productor 4
>>> Buffer envía 3 de posicion 3 a Consumidor 6
> Consumidor 6 recibe valor 3 de Buffer 
>>> Buffer envía 3 de posicion 2 a Consumidor 7
> Consumidor 7 recibe valor 3 de Buffer 
> Consumidor 9 recibe valor 1 de Buffer 
>>> Buffer envía 1 de posicion 1 a Consumidor 9
>>> Buffer envía 1 de posicion 0 a Consumidor 8
> Consumidor 8 recibe valor 1 de Buffer

dave95@G6-DVX:~/Scrivania/MPI/Practica$ 
*/
