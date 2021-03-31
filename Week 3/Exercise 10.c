#include <stdio.h>

/* Complete the function so that it combines nr1 and nr2 into 
a unsigned char and return it. Do NOT use printf or scanf */

unsigned char twoBecomeOne(int nr1, int nr2)
{
    unsigned char twoNumbers = 0;

    int nr[2] = {nr1, nr2};

    for (int i = 0; i < 2; i++)
    {
        if (nr[i] < 0)
        {
            nr[i] = 0;
        }
        else if (nr[i] > 15)
        {
            nr[i] = 15;
        }
    }

    twoNumbers = nr[0];
    twoNumbers |= nr[1] << 4;

    return twoNumbers;
}

int main()
{
    printf("twoNumbers = %d\n", twoBecomeOne(2, 2));    // twoNumbers = 34
    printf("twoNumbers = %d\n", twoBecomeOne(12, -5));  // twoNumbers = 12
    printf("twoNumbers = %d\n", twoBecomeOne(224, 88)); //twoNumbers = 255
    printf("twoNumbers = %d\n", twoBecomeOne(16, 8));   // twoNumbers = 143
    return 0;
}