#include "crono.h"

extrae_timer_tag tags_array[NUMBER_OF_TAGS];
#ifndef HAVE_SYNC_ADD_AND_FETCH
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
#endif

/******************************************************************************
 ***  INTERVAL API
 ******************************************************************************/

/**
 *	Call the function new_timer which creates new_timer
 *	
 *	@param tag Tag identifier
 *	
 *	@return Pointer to the new interval timer  
 */
extrae_timer_t* new_interval(int tag)
{
	return new_timer(tag, 0);
}

/**
 *	Call the function start_timer which starts the interval timer t
 *
 * 	@param t extrae_timer_t Pointer to the interval timer to start
 *
 */
void start_interval(extrae_timer_t *t)
{
	start_timer(t);
}

/**
 *	Call the function stop_interval which stops the interval timer t if
 *	it has not been stopped before
 *
 *	@param t extrae_timer_t Pointer to the interval timer to stop
 *
 *	@return Difference between start time and stop time in nanoseconds
 */
unsigned long long stop_interval(extrae_timer_t *t)
{
	return stop_timer(t);
}

/**
 *	Call the function free_timer which frees the interval timer t
 *
 *	@param t Pointer to interval timer we want to free
 */
void free_interval(extrae_timer_t *t)
{
	free_timer(t);
}

/******************************************************************************
 ***  COUNTDOWN API
 ******************************************************************************/

/**
 *	Call the function new_timer which creates a new countdown timer
 *	
 *	@param tag Tag identifier
 *	
 *	@return Pointer to the new countdown timer  
 */
extrae_timer_t* new_countdown(int tag, int timeout)
{
	return new_timer(tag, timeout);
}

/**
 *	Call the function start_timer which starts the countdown timer t
 *
 * 	@param t Pointer to the countdown timer we want to start
 *
 */
void start_countdown(extrae_timer_t *t)
{
	start_timer(t);

}

/**
 *	Call the function finished that checks if a countdown timer has finished
 *
 *	@param t Pointer to the countdown timer we want to check
 *
 *	@return 0 if countdown is keeps running and 1 if it has already finished
 */
int finished_countdown(extrae_timer_t *t)
{
	return finished(t);
}

/**
 *	Call the function free_timer which frees a countdown timer
 *
 *	@param t Pointer to the countdown timer we want to free
 */
void free_countdown(extrae_timer_t *t)
{
	free_timer(t);
}

/******************************************************************************
 ***  COMMON API
 ******************************************************************************/
/**
 * Allocates a new timer and set the default values
 *
 * @param tag Tag identifier
 * @param timeout Will be zero if timer is an interval, otherwise, 
 * if timer is a countdown it will be greater than zero
 *
 * @return Pointer to the new timer
 */
extrae_timer_t* new_timer(int tag, int timeout)
{
	extrae_timer_t *t = (extrae_timer_t*)malloc(sizeof(extrae_timer_t));
	//extrae_timer_t *t = (extrae_timer_t*)calloc(1, sizeof(extrae_timer_t));
	t->t1 = 0;
	t->t2 = 0;
	t->delta = 0;
	t->tag = tag;	
	t->timeout = timeout;
	return t;
}

/**
 *	Free the timer pointer if it is not NULL
 *
 *	@param t Pointer to the timer we want to free
 *
 */
void free_timer(extrae_timer_t *t){
	if (t != NULL)
	{
	  free(t);
	}
	//*t = NULL;
}

/*
 *	Initialize the tag array with the default values
 */
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

/**
 *	Starts the timer
 *
 *	@param t Pointer to the timer we want to start
 *
 */
void start_timer(extrae_timer_t *t)
{
	//check if the timer is initialized
	if(t != NULL)
	{
    	struct timeval start_time;
		//Initialize delta and get start time
		t->t2 = 0;
		gettimeofday(&start_time, NULL);
		t->t1 = getNanos(&start_time);
	} else {
		fprintf(stderr,"ERROR: Timer is not initializated,\n please call new_interval/new_countdown first\n");
	}
}

/**
 *	Stops a timer if it is still running
 *
 *	@param t Pointer to the timer we want to stop
 *
 *	@return Difference between start time and stop time of a timer
 */
unsigned long long stop_timer(extrae_timer_t *t)
{
	//Check if the timer is initialized
	if(t != NULL)
	{
		//Check timer is still running
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
#ifdef HAVE_SYNC_ADD_AND_FETCH
				__sync_add_and_fetch(&tags_array[t->tag].accumulatedTime, t->delta);
				__sync_add_and_fetch(&tags_array[t->tag].count, 1);
#else
				pthread_mutex_lock(&mutex);
				tags_array[t->tag].accumulatedTime += t->delta;
				tags_array[t->tag].count++;
				pthread_mutex_unlock(&mutex);
#endif
			}
			return t->delta;
		} else {
			fprintf(stderr,"ERROR: Timer already stopped, please call start_interval/start_countdown before if you want to reset the timer\n");
		}
	} else {
		fprintf(stderr,"ERROR: Timer is not initializated,\n please call new_interval/new_countdown first\n");
	}
}

/**
 *	Check if the timer has finished
 *
 *	@param t Pointer to the timer we want to check
 *
 *	@return Return 0 if timer keeps running, otherwise return 1
 */
int finished(extrae_timer_t *t)
{
	//Check if the timer is initialized
	if(t != NULL)
	{
		struct timeval now;
		gettimeofday(&now, NULL);
		if(getNanos(&now) >= t->t1 + t->timeout) return 1;
		return 0;
	} else {
		return -1;
	}

}

/**
 *	Get the time in nanoseconds from a timeval struct
 *
 *	@param t Pointer to the struct timeval
 *
 *	@return Time in nanoseconds
 *
 */
long getNanos(struct timeval *t)
{
	return ((t->tv_sec*1000000000L)+(t->tv_usec*1000L));
}

/**
 *	Print the accumulated information of a tag	
 *
 *	@param tag Tag identifier
 *	
 */
void report(int tag)
{
	if(tags_array[tag].count > 0)
	{
		printf("Reporting tag %d\n", tag);
		tags_array[tag].average = tags_array[tag].accumulatedTime / tags_array[tag].count;
		printf("AccumulatedTime(ns): %lli Count: %d Average(ns): %lli\n", tags_array[tag].accumulatedTime, tags_array[tag].count, tags_array[tag].average);
	}
}

/*
 *	Print ALL accumulated information
 */
void reportAll()
{
	printf("Reporting All\n");
	for(int i=0; i<NUMBER_OF_TAGS; i++)
	{
		report(i);
	}	
}
