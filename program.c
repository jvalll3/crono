#include <stdio.h>
#include <unistd.h>
#include "crono.h"
#include <mpi.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int rank, size;
		
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);

	init();

	#pragma omp parallel
	{

		//extrae_timer_t t = EXTRAE_TIMER_INITIALIZER;
		extrae_timer_t *t = NULL;
		t = newTimer(TAG1, 1, 1);
		start(t);
		sleep(1);	
		stop(t);
	}
	
	//report(TAG1);
	reportAll();
	MPI_Finalize();	
	

	return 0;
}
