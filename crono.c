#include "crono.h"

extrae_timer_tag tags_array[NUMBER_OF_TAGS];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/******************************************************************************
 ***  INTERVAL API
 ******************************************************************************/

extrae_timer_t* new_interval(int tag)
{
	return new_timer(tag, 0);
}

void start_interval(extrae_timer_t *t)
{
	start_timer(t);
}

unsigned long long stop_interval(extrae_timer_t *t)
{
	return stop_timer(t);
}

void free_interval(extrae_timer_t *t)
{
	free_timer(t);
}

/******************************************************************************
 ***  COUNTDOWN API
 ******************************************************************************/

extrae_timer_t* new_countdown(int tag, int timeout)
{
	return new_timer(tag, timeout);
}

void start_countdown(extrae_timer_t *t)
{
	start_timer(t);

}

int finished_countdown(extrae_timer_t *t)
{
	return finished(t);
}

void free_countdown(extrae_timer_t *t)
{
	free_timer(t);
}

/******************************************************************************
 ***  COMMON API
 ******************************************************************************/

extrae_timer_t* new_timer(int tag, int timeout)
{
	extrae_timer_t *t = (extrae_timer_t*)malloc(sizeof(extrae_timer_t));
	t->tag = tag;	
	t->timeout = timeout;
	return t;
}

void free_timer(extrae_timer_t *t){
	free(t);
}


void init()
{
	//Initialize the tag array with default values
	for(int i=0;i< NUMBER_OF_TAGS;i++)
	{
		tags_array[i].count = 0;
		tags_array[i].accumulatedTime = 0;
		tags_array[i].average = 0;
	}
}

void start_timer(extrae_timer_t *t)
{
	//if the timer is not running
	if(t->t1 == 0)
	{	
    	struct timeval start_time;
		//Initialize delta and get start time
		t->delta = 0;
		gettimeofday(&start_time, NULL);
		t->t1 = getNanos(&start_time);
	}
}


<<<<<<< HEAD
unsigned long long stop_timer(extrae_timer_t *t)
=======
void stop_timer(extrae_timer_t *t)
>>>>>>> cac274dca51b77e2774c0e7ff482ae54f6b51575
{
	//if the timer is still running
	if(t->t2 == 0)
	{
    	struct timeval stop_time;
		//Get end time and calculate time difference
		gettimeofday(&stop_time, NULL);
		t->t2 = getNanos(&stop_time);
		t->delta = t->t2 - t->t1;
		if(t->timeout == 0)
		{
			//Update de tag array with the timer values
<<<<<<< HEAD
			/*pthread_mutex_lock(&mutex);
			tags_array[t->tag].accumulatedTime += t->delta;
			tags_array[t->tag].count++;
			pthread_mutex_unlock(&mutex);*/
			__sync_add_and_fetch(&tags_array[t->tag].accumulatedTime, t->delta);
			__sync_add_and_fetch(&tags_array[t->tag].count, 1);
		}
		return t->delta;
=======
			pthread_mutex_lock(&mutex);
			tags_array[t->tag].accumulatedTime += t->delta;
			tags_array[t->tag].count++;
			pthread_mutex_unlock(&mutex);
		}
>>>>>>> cac274dca51b77e2774c0e7ff482ae54f6b51575
	}
}

int finished(extrae_timer_t *t)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	if(getNanos(&now) >= t->t1 + t->timeout) return 1;
	return 0;

}

long getNanos(struct timeval *t)
{
	return ((t->tv_sec*1000000000L)+(t->tv_usec*1000L));
}

void report(int tag)
{
	if(tags_array[tag].count > 0)
	{
		printf("Reporting tag %d\n", tag);
		tags_array[tag].average = tags_array[tag].accumulatedTime / tags_array[tag].count;
		printf("AccumulatedTime(ns): %lli Count: %d Average(ns): %lli\n", tags_array[tag].accumulatedTime, tags_array[tag].count, tags_array[tag].average);
	}
}

void reportAll()
{
	printf("Reporting All\n");
	for(int i=0; i<NUMBER_OF_TAGS; i++)
	{
		report(i);
	}	
}
