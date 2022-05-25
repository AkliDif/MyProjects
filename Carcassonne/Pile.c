#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"


int empiler_tuile(Pile* P, Tuile T)
{
    if (P->nb_tuile == P->nb_tuile_max)
        return 0;
    else 
    {
        P->T[P->nb_tuile].cote_A->type = T.cote_A->type;
        P->T[P->nb_tuile].cote_B->type = T.cote_B->type;
        P->T[P->nb_tuile].cote_C->type = T.cote_C->type;
        P->T[P->nb_tuile].cote_D->type = T.cote_D->type;
        P->T[P->nb_tuile].cote_E->type = T.cote_E->type;
        P->T[P->nb_tuile].id_tuile = T.id_tuile;
        P->nb_tuile++;

        return 1;
    }
}


Pile* init_pile ()
{
    Pile* P = (Pile*)malloc (sizeof(Pile));
    P->nb_tuile_max = NB_TUILE_MAX;
    P->nb_tuile = 0;
    P->T = (Tuile*)malloc (P->nb_tuile_max*sizeof(Tuile));
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        P->T[i].cote_A = (Cote*)malloc(sizeof(Cote));
        P->T[i].cote_B = (Cote*)malloc(sizeof(Cote));
        P->T[i].cote_C = (Cote*)malloc(sizeof(Cote));
        P->T[i].cote_D = (Cote*)malloc(sizeof(Cote));
        P->T[i].cote_E = (Cote*)malloc(sizeof(Cote));

    }
    return P;
}


void free_pile (Pile* P)
{
    for (int i=0 ; i < NB_TUILE_MAX ; i++)
    {
        free(P->T[i].cote_A);
        free(P->T[i].cote_B);
        free(P->T[i].cote_C);
        free(P->T[i].cote_D);
        free(P->T[i].cote_E);
    }

    free(P->T);
    P->T = NULL;
    P->nb_tuile = 0;
}

Pile* creer_pile ()
{ 
    Pile* P = init_pile ();
    Tuile* temp;
    temp = get_tiles_from_file ();
    
    melanger_tuile (temp);

    for (int i=1 ; i<NB_TUILE_MAX ; i++)
    {
        if (!empiler_tuile (P, temp[i]))
        {
            fprintf (stderr, "Error. Création de pile echoué\n");
            exit(1);
        }
    }
    if (!empiler_tuile (P, temp[0]))
    {
        fprintf (stderr, "Error. Création de pile echoué\n");
        exit(1);
    }
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        free (temp[i].cote_A);
        free (temp[i].cote_B);
        free (temp[i].cote_C);
        free (temp[i].cote_D);
        free (temp[i].cote_E);
    }

    free (temp);
    
    return P;
}

Tuile* depiler_tuile (Pile *P)
{
    if (P == NULL)
    {
        fprintf (stderr, "Error : Pile introuvable\n");
        return NULL;
    }
    if (P->nb_tuile == 0)
    {
        printf ("La pile est vide\n");
        return NULL;
    }
    Tuile* T = creer_tuile (P->T[P->nb_tuile-1].cote_A->type, P->T[P->nb_tuile-1].cote_B->type, P->T[P->nb_tuile-1].cote_C->type, P->T[P->nb_tuile-1].cote_D->type, P->T[P->nb_tuile-1].cote_E->type, P->T[P->nb_tuile-1].id_tuile);
    P->nb_tuile --;
    return T;
}


void afficher_pile (Pile* P)
{
    int i = P->nb_tuile-1;
    while (i >= 0)
    {
        printf ("%c %c %c %c %c \n", P->T[i].cote_A->type, P->T[i].cote_B->type, P->T[i].cote_C->type, P->T[i].cote_D->type, P->T[i].cote_E->type);
        i--;
    }
}