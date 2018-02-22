#include <stdio.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include "crono.h"
#include <mpi.h>
#include <omp.h>

#define ARRAYSIZE 10000000

int main(int argc, char* argv[])
{
	int rank, size;

	/*MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);*/
	
	int *a = (int*) malloc(sizeof(int)*ARRAYSIZE);
	int *b = (int*) malloc(sizeof(int)*ARRAYSIZE);

	srand(time(NULL));
	init();
	#pragma omp parallel
	{
		extrae_timer_t *t1 = NULL;
		t1 = new_interval(FIRST_LOOP);
		/*pid_t tid;
		tid = syscall(SYS_gettid);
		printf("tid = %d\n",tid);*/
		start_interval(t1);
		for(int i=0; i<ARRAYSIZE; i++)
		{
			a[i] = 100;
			//a[i] = (int)rand();
		}
		stop_interval(t1);
		start_interval(t1);
		stop_interval(t1);
	   	free_interval(t1);	
	}

	extrae_timer_t *t2 = NULL;
	t2 = new_interval(SECOND_LOOP);
	start_interval(t2);
	for(int i=0; i<ARRAYSIZE; i++)
	{
		b[i] = (a[i] / 2);
	}
	stop_interval(t2);
	stop_interval(t2);
	free_interval(t2);

	
	#pragma omp parallel
	{
		extrae_timer_t t3 = EXTRAE_INTERVAL_INITIALIZER(THIRD_LOOP);
		start_interval(&t3);
		sleep(1);	
		stop_interval(&t3);
	}
	
	extrae_timer_t *t4 = NULL;
	t4 = new_countdown(COUNTDOWN, 1500000);
	start_countdown(t4);
	sleep(1);
	printf("finished = %d\n", finished_countdown(t4));
	sleep(1);
	printf("finished = %d\n", finished_countdown(t4));
	sleep(1);
	printf("finished = %d\n", finished_countdown(t4));
	free_countdown(t4);

	free(a);
	free(b);
	reportAll();
	//MPI_Finalize();	
	

	return 0;
}
