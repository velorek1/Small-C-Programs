//A timer implementation in C. Coded by velorek. 

#include <stdio.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

typedef struct nTimer{
    int  ms; //milliseconds
    long oldtime; //to calculate increment in time
    long ticks; //how many ticks have been made
} NTIMER;

/* Miliseconds timer */
int timerC(NTIMER *mytimer1);
void init_timer(NTIMER *mytimer, int ms);
void stop_timer(NTIMER *mytimer);
void resume_timer(NTIMER *mytimer);

/* Milliseconds timer */
int timerC(NTIMER *mytimer1){
clock_t difference = 0;
long res;
 if (mytimer1->ticks == -1) return 0;
 if (mytimer1->ticks == 0){
    //First tick, set up values
    mytimer1->oldtime = clock();
    mytimer1->ticks = 1;
    return 1;
  } else{
    //subsequent ticks
    difference =  clock() - mytimer1->oldtime;
    res = difference * 1000 / CLOCKS_PER_SEC;
    if (res < mytimer1->ms) {
      return 0;
    } else{
      mytimer1->oldtime = clock();
      mytimer1->ticks = mytimer1->ticks + 1;
      return 1;
    }
  }
}

void init_timer(NTIMER *mytimer1, int ms){
//Init routine
   mytimer1->ticks = 0;
   mytimer1->ms=ms;
}

void stop_timer(NTIMER *mytimer1){
//Stop routine
   mytimer1->ticks=-1;
}
void resume_timer(NTIMER *mytimer1){
//Resume routine
   mytimer1->ticks=0;
}

//main
int main(){
   NTIMER timer1;
   init_timer(&timer1,500);
   //Loop until the timer ticks 10 times
   do{
      if (timerC(&timer1) == TRUE) {
         printf("Printing this message every 500 milliseconds. [%ld]\n",timer1.ticks);
      }
   } while (timer1.ticks<10);
  return 0;
}
