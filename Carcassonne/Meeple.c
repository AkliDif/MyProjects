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
        tab_meeple[i].color = num_player;
        tab_meeple[i].num_player = 0;
        tab_meeple[i].pose = 0;
    }
    return tab_meeple;
}


int pose_meeple (Tuile *T, Meeple *M, char cote)
{
    int pose = 0;
    switch (cote)
    {
    case 'A':
        if (T->cote_A->occupe == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_A->Pion = M;
            T->cote_A->occupe = 1;
            pose = 1;
        }
        break;

    case 'B':
        if (T->cote_B->occupe == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_B->Pion = M;
            T->cote_B->occupe = 1;
            pose = 1;
        }
        break;

    case 'C':
        if (T->cote_C->occupe == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_C->Pion = M;
            T->cote_C->occupe = 1;
            pose = 1;
        }
        break;
    
    case 'D':
        if (T->cote_D->occupe == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_D->Pion = M;
            T->cote_D->occupe = 1;
            pose = 1;
        }
        break;
    
    case 'E':
        if (T->cote_E->occupe == 1)
            printf("Il y a déjà un meeple\n");
        else
        {
            T->cote_E->Pion = M;
            T->cote_E->occupe = 1;
            pose = 1;
        }
        break;
    default:
        pose = 0;
        break;
    }

    if (strcmp (T->cote_E->type, T->cote_A->type) == 0)
    {
        if (T->cote_E->occupe == 1 || T->cote_A->occupe == 1)
        {
            T->cote_E->occupe == 1;
            T->cote_A->occupe == 1;
        }
    }

    if (strcmp (T->cote_E->type, T->cote_B->type) == 0)
    {
        if (T->cote_E->occupe == 1 || T->cote_B->occupe == 1)
        {
            T->cote_E->occupe == 1;
            T->cote_B->occupe == 1;
        }
    }

    if (strcmp (T->cote_E->type, T->cote_C->type) == 0)
    {
        if (T->cote_E->occupe == 1 || T->cote_C->occupe == 1)
        {
            T->cote_E->occupe == 1;
            T->cote_C->occupe == 1;
        }
    }

    if (strcmp (T->cote_E->type, T->cote_D->type) == 0)
    {
        if (T->cote_E->occupe == 1 || T->cote_D->occupe == 1)
        {
            T->cote_E->occupe == 1;
            T->cote_D->occupe == 1;
        }
    }


    return pose;
}