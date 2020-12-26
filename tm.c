#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nTimer{
        int  ms; //milliseconds
	long oldtime; //to calculate increment in time
	long oldticks; //to calculate increment in no. of ticks
	long ticks; //how many ticks have been made
	long elapsed; //total time elapsed in ms
} NTIMER;

/* Miliseconds timer */
int timer1(NTIMER *mytimer1){
struct timespec ts;
long get_ms;
long res, res2;

 if (mytimer1->ticks == 0){
    //First tick, set up values
    mytimer1->oldtime = 0;
    mytimer1->ticks = 1;
    mytimer1->oldticks = 1;
    mytimer1->elapsed = 0;
    return 1;
  } else{
    //subsequent ticks
    timespec_get(&ts, TIME_UTC);
    //Getms gets 10^-3 milleseconds
    get_ms = (ts.tv_nsec % 10000000) / 1000000;
    //Add a millisecond every time a change is detected in 10^-3
    res = get_ms-mytimer1->oldtime;
    if(abs(res) >= 1){
      mytimer1->elapsed = mytimer1->elapsed + 1;
      mytimer1->oldtime = get_ms;
    }
    //Calculate tick by divided time elapsed with ms requested
    res2 = mytimer1->elapsed / mytimer1->ms;
    if (res2 == mytimer1->oldticks) {  
      mytimer1->ticks = mytimer1->ticks + 1;
      mytimer1->oldticks = mytimer1->oldticks+1;
      return 1;
    } else
      return 0;
  }
}

int main(){

NTIMER mytimer1;
int i=0;
char msg[13] = "Hello there!\0";

/* INIT TIMER */
//mytimer1.ticks must be set to 0 at start.
mytimer1.ms = 30;
mytimer1.ticks = 0;

  do{
      if (timer1(&mytimer1) == 1) { 
          printf("%c\n", msg[i]);
	  i++;
       } 
  } while (mytimer1.ticks < 13);
  printf("Total elapsed time in ms: %ld", mytimer1.elapsed);
}

