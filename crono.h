#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define EXTRAE_INTERVAL_INITIALIZER(tag) {0,0,0,tag,0} 
#define EXTRAE_COUNTDOWN_INITIALIZER(tag, timeout) {0,0,0,tag,timeout} 

enum tags                                                                           
{
	FIRST_LOOP = 0,
	SECOND_LOOP,
	THIRD_LOOP,
	COUNTDOWN,
	NUMBER_OF_TAGS
}; 

struct tag
{
	int count;
	unsigned long long accumulatedTime;
	unsigned long long average;
};

struct e
{
	unsigned long long t1;
	unsigned long long t2;
	unsigned long long delta;
	int tag;
	int timeout;
};

typedef struct e extrae_timer_t;

typedef struct tag extrae_timer_tag;


//INTERVAL API
extrae_timer_t* new_interval(int tag);
void start_interval(extrae_timer_t *t);
unsigned long long stop_interval(extrae_timer_t *t);
void free_interval(extrae_timer_t *t);


//COUNTDOWN API
extrae_timer_t* new_countdown(int tag, int timeout);
void start_countdown(extrae_timer_t *t);
int finished_countdown(extrae_timer_t *t);
void free_countdown(extrae_timer_t *t);

//COMMON API
extrae_timer_t* new_timer(int tag, int timeout);
void init();
long getNanos(struct timeval *t);
void start_timer(extrae_timer_t *t);
unsigned long long stop_timer(extrae_timer_t *t);
int finished(extrae_timer_t *t);
void report(int tag);
void reportAll();
void free_timer(extrae_timer_t *t);

