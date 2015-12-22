#include "mpi.h"
#include <iostream>
using namespace std;
 
int main(int argc, char *argv[]) {
 int rank, size, vecino, mivalor, valor;
 MPI_Status status;
 MPI_Request request_send,request_recv;
 MPI_Init(&argc, &argv);
 MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 MPI_Comm_size( MPI_COMM_WORLD, &size );
 mivalor=rank*(rank+1);
 if (rank %2 == 0) vecino=rank+1;
    else vecino=rank-1;
 // Las siguientes operaciones pueden aparecer en cualquier orden   
 MPI_Irecv( &valor, 1, MPI_INT, vecino, 0, MPI_COMM_WORLD,&request_recv);
 MPI_Isend( &mivalor, 1, MPI_INT, vecino, 0, MPI_COMM_WORLD,&request_send ); 
 MPI_Wait(&request_send,&status);   
 MPI_Wait(&request_recv,&status);      
 cout<< "Soy el proceso "<<rank<<" y he recibido "<<valor<<endl;
 MPI_Finalize(); return 0;
}







