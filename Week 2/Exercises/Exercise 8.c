#include <stdio.h>
#include <stdlib.h>

void MinSort(int *array)
{
    int size = 0;

    while (array[size] != 0)
    {
        size++;
    }

    int temp = 0;

    for (int i = 0; i < size - 1; i++)
    {
        for (int j = i + 1; j < size; j++)
        {
            if (array[j] < array[i])
            {
                temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    } 
}

void minput()
{
    int *inputList = malloc(sizeof(int));
    int currentIndex = 0;
    int input = -1;

    while (input != 0)
    {
        scanf("%d", &input);

        inputList[currentIndex] = input;
        currentIndex++;
        inputList = realloc(inputList, (currentIndex + 1) * sizeof(int));
    }

    MinSort(inputList);

    for (int i = 0; i < currentIndex - 1; i++)
    {
        printf("%d\n", inputList[i]);
    }
}