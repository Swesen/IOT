#include <stdio.h>

int main(){
    float number;
    printf("The rounded number is: ");
    
    scanf(" %f", &number);
    printf("%.2f", number);
    
    return 0;
}