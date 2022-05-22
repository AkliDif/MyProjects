#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Meeple.h"
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"


Meeple* create_meeples (int num_player)
{
    Meeple* tab_meeple = (Meeple*)malloc (NB_MAX_MEEPLE * sizeof(Meeple));
    for (int i=0 ; i<NB_MAX_MEEPLE ; i++)
    {
        tab_meeple[i].num_player = num_player;
    }
    return tab_meeple;
}


int pose_meeple (Tuile *T, Meeple *M, int cote, int num_player)
{
    int pose = 0;
    switch (cote)
    {
    case 1:
        if (T->cote_A->vide != 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_A->Pion = M;
            T->cote_A->vide = 0;

            pose = 1;
        }
        break;

    case 2:
        if (T->cote_B->vide != 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_B->Pion = M;
            T->cote_B->vide = 0;
            T->cote_A->Pion->coleur = num_player;
            pose = 1;
        }
        break;

    case 3:
        if (T->cote_C->vide == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_C->Pion = M;
            T->cote_C->vide = 0;
            T->cote_A->Pion->coleur = num_player;
            pose = 1;
        }
        break;
    
    case 4:
        if (T->cote_D->vide == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_D->Pion = M;
            T->cote_D->vide = 0;
            T->cote_A->Pion->coleur = num_player;
            pose = 1;
        }
        break;
    
    case 5:
        if (T->cote_E->vide == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_E->Pion = M;
            T->cote_E->vide = 0;
            T->cote_A->Pion->coleur = num_player;
            pose = 1;
        }
        break;
    default:
        pose = 0;
        break;
    }

    if (strcmp (T->cote_E->type, T->cote_A->type) == 0)
    {
        if (T->cote_E->vide == 0 || T->cote_A->vide == 0)
        {
            T->cote_E->vide == 0;
            T->cote_A->vide == 0;
        }
    }

    if (strcmp (T->cote_E->type, T->cote_B->type) == 0)
    {
        if (T->cote_E->vide == 0 || T->cote_B->vide == 0)
        {
            T->cote_E->vide == 0;
            T->cote_B->vide == 0;
        }
    }

    if (strcmp (T->cote_E->type, T->cote_C->type) == 0)
    {
        if (T->cote_E->vide == 0 || T->cote_C->vide == 0)
        {
            T->cote_E->vide == 0;
            T->cote_C->vide == 0;
        }
    }

    if (strcmp (T->cote_E->type, T->cote_D->type) == 0)
    {
        if (T->cote_E->vide == 0 || T->cote_D->vide == 0)
        {
            T->cote_E->vide == 0;
            T->cote_D->vide == 0;
        }
    }

    return pose;
}