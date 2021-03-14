#include <stdio.h>

int main(){
    char firstInitial;
    char lastInitial;
    int age;
    int favoriteNumber;

    printf("What letter does your first name begin with?\n");
    scanf(" %c", &firstInitial);

    printf("What letter does yor surname begin with?\n");
    scanf(" %c", &lastInitial);

    printf("How old are you?\n");
    scanf(" %d", &age);

    printf("What is your favorite number?\n");
    scanf(" %d", &favoriteNumber);

    printf("Your initials are: %c.%c\nYou are %d old\nAnd your favorite number is %d\n", firstInitial, lastInitial, age, favoriteNumber);
    return 0;
}