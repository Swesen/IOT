#include <stdio.h>

void isPrime(int nr)
{
    for (int i = 2; i < nr; i++)
    {
        if (nr % i == 0)
        {
            printf("FALSE");
            return;
        }
    }
    printf("TRUE");
}