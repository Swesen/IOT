int main()
{
    int myTable[10][10];

    for (int x = 0; x < 10; x++)
    {
        for (int y = 0; y < 10; y++)
        {
            myTable[x][y] = (x + 1) * (y + 1);
        }
    }

    table(myTable);
    return 0;
}