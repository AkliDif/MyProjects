#include <stdio.h>
#include <math.h>

int main (void)
{
    int i, j;
    for (i=0 ; i<3 ; i++)
        for (j=0 ; j<3 ; j++)
        {
            if (j == 1 )
            {
                printf ("%d\n", i);
                break;
            }
        }

    return 0;
}