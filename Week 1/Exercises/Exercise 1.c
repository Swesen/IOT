#include <stdio.h>

/* The function below is already defined and ready for use
int bigOrSmall(int number){}

It will return -1 or 0 or 1
*/
int myNumber = 75;

int bigOrSmall(int number){
    if (number < myNumber)
    {
        return -1;
    } else if (number > myNumber)
    {
        return 1;
    } else
    {
        return 0;
    }
}

int main(){
    int answer;
    int myGuess = 10;
    
    while (myGuess != answer){
        answer = myGuess;
        myGuess = myGuess - bigOrSmall(myGuess);
    }

    printf("%d" ,myGuess);
    return 0;
}