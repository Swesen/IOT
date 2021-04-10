/* Prints all even and odd numbers from 0 to 10 */

#include <stdio.h>

int main(){
   printf("EVEN NUMBERS:\n");
   for (int x = 0; x <= 10; x++){
       if (x % 2 == 0){
           printf("%d\n",x);
       }
   }
   printf("ODD NUMBERS:\n");
   for (int x = 0; x <= 10; x++){
       if (x % 2 != 0){
           printf("%d\n",x);
       }
   }
    
    return 0;
}