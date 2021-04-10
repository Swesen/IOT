#include <stdio.h>
#include <math.h>
/* Finish the function. Use printf() to answer. 
Both stdio.h and math.h have already been included */

void distance(float *p1, float *p2)
{
    printf("%.2f", sqrt(pow(p2[0] - p1[0], 2) + pow(p2[1] - p1[1], 2) + pow(p2[2] - p1[2], 2)));
}