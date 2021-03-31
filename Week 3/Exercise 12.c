#include <stdio.h>
#include <stdlib.h>

//YOU MUST USE THIS STRUCT WITHOUT CHANGES TO THE STRUCTURE
typedef struct nod
{
    int value;
    struct nod *left;
    struct nod *right;

} node;

node *createNode(int value){
    node *node = malloc(sizeof(node));
    node->value = value,
    node->left = NULL;
    node->right = NULL;
    return node;
}

void placeInTree(node *root, int newValue)
{
    node *currentNode = root;

    for (;;)
    {
        if (newValue < currentNode->value)
        {
            if (currentNode->left == NULL)
            {
                currentNode->left = createNode(newValue);
                return;
            }

            currentNode = currentNode->left;
        }
        else
        {
            if (currentNode->right == NULL)
            {
                currentNode->right = createNode(newValue);
                return;
            }
            currentNode = currentNode->right;
        }
    }
}

node *makeTree(int myArray[])
{

    int arrayLength = myArray[0];

    node *root = createNode(myArray[1]);

    for (int i = 2; i < arrayLength; i++)
    {
        placeInTree(root, myArray[i]);
    }

    return root;
}