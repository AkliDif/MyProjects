#include <stdio.h>
#include <stdlib.h>
    

int print (int a, int b)
{
    int cpt = 0;
    if (a == 0 || b == 0)
        return cpt;
    return print (a-1, b-1) + 1;
}

int main (void)
{
    int a = 5, b = 5;

    int cpt = print (a, b);
    printf ("%d\n", cpt);
    return 0;
}