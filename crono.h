#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define EXTRAE_TIMER_INITIALIZER {0,0,0,0} 

enum tags                                                                           
{
	TAG1 = 0,
	TAG2,
	NUMBER_OF_TAGS
}; 

struct tag
{
	int count;
	float accumulatedTime;
	float average;
};

struct e
{
	struct timeval t1;
	struct timeval t2;
	float delta;
	int tag;
	int report;
	int timeout;
};

typedef struct e extrae_timer_t;

typedef struct tag extrae_timer_tag;

extrae_timer_t* newTimer(int tag, int report, int timeout);
void init();
void start(extrae_timer_t *t);
float stop(extrae_timer_t *t);
int finished(extrae_timer_t *t);
void report(int tag);
void reportAll();
