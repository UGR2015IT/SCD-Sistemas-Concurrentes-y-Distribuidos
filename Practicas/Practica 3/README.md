#### Important information for the MPI Library

--------------------------------------

The first thing that needs to be done is installing the OpenMPI Library:
		
		sudo apt-get install openmpi-bin openssh-client openssh-server libopenmpi-dbg libopenmpi-dev

<p>
Once done that is done, keep in mind this simple steps:
- to compile the code and then run it:

		mpicxx -o filename filename.cpp
		mpirun -np numberOfProcesses filename
- the code must include:

		include "mpi.h"
- in the main function, among the other things there has to be:

		int main(int argc, char *argv[]){
			int rank, size, value;
			MPI_STATUS status;	//Mandatory for the Receive
			MPI_Init(&argc,&argv);
			MPI_Comm_rank(MPI_COMM_WORLD, &rank);
			MPI_Comm_size(MPI_COMM_WORLD, &size);
			// ...
		}
