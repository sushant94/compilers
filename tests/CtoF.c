#include<stdio.h>
 
int main() {
 
   float fahrenheit, celsius;
   int lower, upper, step;
 
   lower = 0;
   upper = 200;
   step = 20;
 
   printf("  C\tF");
   printf("\n------------");
 
   celsius = lower;
   while (celsius <= upper) {
      fahrenheit = (9.0 / 5.0) * celsius + 32.0;
      printf("\n%3.0f %6.1f", celsius, fahrenheit);
      celsius = celsius + step;
   }
 
   return 0;
}
