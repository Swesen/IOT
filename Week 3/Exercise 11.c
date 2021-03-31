#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Use printf for your prints.

void printB(unsigned int toBinary)
{
    for (unsigned int i = 1 << 31; i > 0; i >>= 1)
    {
        printf("%u", (toBinary & i) == i);
    }
}

int main()
{
    printB(32);         // 00000000000000000000000000100000
    printB(44);         // 00000000000000000000000000101100
    printB(128);        // 00000000000000000000000010000000
    printB(4294967295); // 11111111111111111111111111111111

    return 0;
}
