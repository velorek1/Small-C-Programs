#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct nTimer{
        int  ms;
	long oldtime;
	long oldticks;
	long ticks;
	long elapsed;
} NTIMER;

/* Miliseconds timer */
int timer1(NTIMER *mytimer1){
struct timespec ts;
long tim1,tim2;
long res, res2, ms;
 if (mytimer1->ticks == 0){
    mytimer1->oldtime = 0;
    mytimer1->ticks = 1;
    return 1;
  } else{
    timespec_get(&ts, TIME_UTC);
    tim1 = (ts.tv_nsec - (ts.tv_nsec % 100000000)) / 1000000;
    tim2 = (ts.tv_nsec - (ts.tv_nsec % 10000000)) / 10000000;
    ms = tim1+tim2;
    res = ms-mytimer1->oldtime;
    if(abs(res) >= 50){
      mytimer1->elapsed = mytimer1->elapsed + 50;
      mytimer1->oldtime = ms;
    }
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

/* INIT TIMER */
mytimer1.ms = 250;
mytimer1.ticks = 0;
mytimer1.oldticks = 1;
mytimer1.oldtime = 0;
mytimer1.elapsed = 0;

  do{
      if (timer1(&mytimer1) == 1) { 
          printf("Tick: %ld\n", mytimer1.ticks);
       } 
  } while (mytimer1.ticks <= 10);
  printf("Total elapsed time in ms: %ld", mytimer1.elapsed);
}

