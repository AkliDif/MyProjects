#include <stdio.h>
#include <stdlib.h>
#include "TP1.h"
#include "TP2.h"



int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf (stderr, "Erreur : l’utilisateur doit indiquer le nom d’un fichier texte en parametre. Par exemple :\n./main fichier_texte.txt\n");
        return 1;
    }
    char* string = getStringFromFile (argv[1]);
    unsigned int nb = nbDifferentChar (string);
    couple* tab = getOccurencesList (string, &nb);
    for (int i=0 ; i<nb ; i++)
        printCouple (tab[i]);
    free (string);
    free (tab);
    return 0;
}