#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <string.h>

int main (void)
{
    char a = 'e';
    int b;
    b = (int)a;
    printf ("%c %d \n", a, b);

    return 0;
}