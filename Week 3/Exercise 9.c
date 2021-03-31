#include <stdio.h>

void makeMeOne(int nr, int theOne)
{
    if (theOne > 32)
    {
        theOne = 32;
    }
    else if (theOne < 1)
    {
        theOne = 1;
    }

    nr |= 1 << (theOne - 1);
    printf("%d\n", nr);
}

int main()
{
    makeMeOne(32, 4); // 40
    makeMeOne(0, 8);  // 128
    makeMeOne(1, 0); // 1
    makeMeOne(7, 4);  // 15
    return 0;
}
