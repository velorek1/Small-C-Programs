/* 
Demonstration of a formula that uses MOD to calculate the sum
of a 2-digit integer [10..99]

Description:             
12 -> 1+2 -> 3 ---> 12 - (9*1) = 3
24 -> 2+4 -> 6 ---> 24 - (9*2) = 6
..
99 -> 9+9 -> 18 ---> 99 - (9*9) = 18  
|____________________________^

Adding two digits of a number is the same 
as deducting 9 times the number in the tens from
the original number. 
This formula isolates this number using MOD:
  f(x) = x - ((x - x % 10)/10) * 9 
*/
 

#include <stdio.h>

int main(){
     short i,result;
     printf("[10..99] \n\n");
     for (i=10; i<100; i++){
         result = i - ((i-i %10)/10)*9;
         printf("%i -> %i\n", i, result);   
     }    
  return 0;
}
