#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Meeple.h"
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"

//------------------------------ empilage d'une tuile sur la pile ------------------------------------

Tuile* creer_tuile (char cote_A[10], char cote_B[10], char cote_C[10], char cote_D[10], char cote_E[10], int num)
{
    Tuile* T = (Tuile*)malloc (sizeof(Tuile));
    T->cote_A = (Cote*)malloc(sizeof(Cote));
    T->cote_B = (Cote*)malloc(sizeof(Cote));
    T->cote_C = (Cote*)malloc(sizeof(Cote));
    T->cote_D = (Cote*)malloc(sizeof(Cote));
    T->cote_E = (Cote*)malloc(sizeof(Cote));

    T->cote_A->Pion = (Meeple*)malloc(sizeof(Meeple));
    T->cote_B->Pion = (Meeple*)malloc(sizeof(Meeple));
    T->cote_C->Pion = (Meeple*)malloc(sizeof(Meeple));
    T->cote_D->Pion = (Meeple*)malloc(sizeof(Meeple));
    T->cote_E->Pion = (Meeple*)malloc(sizeof(Meeple));

    strcpy (T->cote_A->type, cote_A);
    strcpy (T->cote_B->type, cote_B);
    strcpy (T->cote_C->type, cote_C);
    strcpy (T->cote_D->type, cote_D);
    strcpy (T->cote_E->type, cote_E);
    T->cote_A->occupe = 0;
    T->cote_B->occupe = 0;
    T->cote_C->occupe = 0;
    T->cote_D->occupe = 0;
    T->cote_E->occupe = 0;
    T->nb = num;
    T->Joue = 1;
    return T;
}


void free_tile (Tuile *T)
{
    free(T->cote_A->Pion);
    free(T->cote_B->Pion);
    free(T->cote_C->Pion);
    free(T->cote_D->Pion);
    free(T->cote_E->Pion);

    free(T->cote_A);
    free(T->cote_B);
    free(T->cote_C);
    free(T->cote_D);
    free(T->cote_E);

    free(T);

}

// recuperer les tuiles depuis un fichier et les stocker dansd un tableau de tuile

Tuile* get_tiles_from_file () 
{
    Tuile* temp =(Tuile*)malloc (NB_TUILE_MAX * sizeof(Tuile));
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        temp[i].cote_A = (Cote*)malloc(sizeof(Cote));
        temp[i].cote_B = (Cote*)malloc(sizeof(Cote));
        temp[i].cote_C = (Cote*)malloc(sizeof(Cote));
        temp[i].cote_D = (Cote*)malloc(sizeof(Cote));
        temp[i].cote_E = (Cote*)malloc(sizeof(Cote));
    }

    int i = 0;
    
    FILE* fichier = fopen ("tiles.txt", "r");
    
    if(fichier == NULL)
    {
        fprintf (stderr, "Error. Ouverture de ficier echoué\n");
        exit(1);
    }

    while(!feof(fichier))
    {
        fscanf(fichier, "%s %s %s %s %s", temp[i].cote_A->type, temp[i].cote_B->type, temp[i].cote_C->type, temp[i].cote_D->type, temp[i].cote_E->type);
        temp[i].cote_A->occupe = 0;
        temp[i].cote_B->occupe = 0;
        temp[i].cote_C->occupe = 0;
        temp[i].cote_D->occupe = 0;
        temp[i].cote_E->occupe = 0;
        temp[i].nb = i+1;
        temp[i].Joue = 0;
        i++;

    }

    fclose (fichier);
    return temp;
}


//melanger le tableau obtenue alratoirement pour avoir une pile differente à chaque execution
void melanger_tuile (Tuile* temp)
{
    srand (time(NULL));
    for (int i = 1 ; i < NB_TUILE_MAX ; i++ ) 
    {
        int j = rand() % 71 +1;           
        Tuile tmp = temp[i];
        temp[i] = temp[j];
        temp[j] = tmp;
    }
}

void print_tile (Tuile *T)
{
    printf ("  %s  \n", T->cote_A->type);
    printf ("%s %s %s\n", T->cote_D->type, T->cote_E->type, T->cote_B->type);
    printf ("  %s  \n", T->cote_C->type);
}


Tuile* rotate_tuile(Tuile* T)
{
    Tuile* temp = creer_tuile (T->cote_B->type, T->cote_C->type, T->cote_D->type, T->cote_A->type, T->cote_E->type, T->nb);
    return temp;
}