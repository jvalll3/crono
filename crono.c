#include "crono.h"

extrae_timer_tag tags_array[NUMBER_OF_TAGS];

extrae_timer_t* newTimer(int tag, int report, int timeout)
{
	extrae_timer_t *t = (extrae_timer_t*)malloc(sizeof(extrae_timer_t));
	t->tag = tag;	
	t->report = report;	
	t->timeout = timeout;	
	return t;
}



void init()
{
	for(int i=0;i< NUMBER_OF_TAGS;i++)
	{
		tags_array[i].count = 0;
		tags_array[i].accumulatedTime = 0;
		tags_array[i].average = 0;
	}
	/*array = (timer**)malloc(sizeof(timer*)*numThreads);
	for(int i = 0; i < numThreads; i++)
	{
		array[i] = (timer*)malloc(sizeof(timer)*NUMBER_OF_TAGS);
	}*/
}



void start(extrae_timer_t *t)
{
	//printf("Start\n");
	//Initialize delta and get start time
	t->delta = 0;
	gettimeofday(&t->t1,NULL);
}

float stop(extrae_timer_t *t)
{
	//printf("Stop\n");
	//Get end time and calculate delta
	gettimeofday(&t->t2,NULL);
	t->delta = (((t->t2.tv_sec - t->t1.tv_sec)*1000000L)+(t->t2.tv_usec - t->t1.tv_usec));
	//Update de tag array with the timer values
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&mutex);
	tags_array[t->tag].accumulatedTime += (((t->t2.tv_sec - t->t1.tv_sec)*1000000L)+(t->t2.tv_usec - t->t1.tv_usec));
	tags_array[t->tag].count++;
	pthread_mutex_unlock(&mutex);
	pthread_mutex_destroy(&mutex);
}

int finished(extrae_timer_t *t)
{
	if(t->t2.tv_sec > 0 && t->t2.tv_usec > 0) return 0;
	return 1;
}


void report(int tag)
{
	if(tags_array[tag].count > 0)
	{
		tags_array[tag].average = tags_array[tag].accumulatedTime / tags_array[tag].count;
	}
	printf("AccumulatedTime(us): %.2f Count: %d Average: %.2f\n", tags_array[tag].accumulatedTime, tags_array[tag].count, tags_array[tag].average);
}

void reportAll()
{
	printf("Reporting All\n");
	for(int i=0; i<NUMBER_OF_TAGS; i++)
	{
		report(i);
	}	
}
