#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"

//------------------------------ empilage d'une tuile sur la pile ------------------------------------

Tuile* creer_tuile (char cote_A, char cote_B, char cote_C, char cote_D, char cote_E, int id_tuile)
{
    Tuile* T = (Tuile*)malloc (sizeof(Tuile));
    T->cote_A = (Cote*)malloc(sizeof(Cote));
    T->cote_B = (Cote*)malloc(sizeof(Cote));
    T->cote_C = (Cote*)malloc(sizeof(Cote));
    T->cote_D = (Cote*)malloc(sizeof(Cote));
    T->cote_E = (Cote*)malloc(sizeof(Cote));


    T->cote_A->type = cote_A;
    T->cote_B->type = cote_B;
    T->cote_C->type = cote_C;
    T->cote_D->type = cote_D;
    T->cote_E->type = cote_E;
    T->cote_A->cotient_meeple = 0;
    T->cote_B->cotient_meeple = 0;
    T->cote_C->cotient_meeple = 0;
    T->cote_D->cotient_meeple = 0;
    T->cote_E->cotient_meeple = 0;
    T->id_tuile = id_tuile;
    return T;
}


void free_tile (Tuile *T)
{

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
        fscanf(fichier, "%c %c %c %c %c %d ", &temp[i].cote_A->type, &temp[i].cote_B->type, &temp[i].cote_C->type, &temp[i].cote_D->type, &temp[i].cote_E->type, &temp[i].id_tuile);
        temp[i].cote_A->cotient_meeple = 0;
        temp[i].cote_B->cotient_meeple = 0;
        temp[i].cote_C->cotient_meeple = 0;
        temp[i].cote_D->cotient_meeple = 0;
        temp[i].cote_E->cotient_meeple = 0;
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
    printf ("  %c  \n", T->cote_A->type);
    printf ("%c %c %c\n", T->cote_D->type, T->cote_E->type, T->cote_B->type);
    printf ("  %c  \n", T->cote_C->type);
}


Tuile* rotate_tuile(Tuile* T)
{

    T = creer_tuile (T->cote_B->type, T->cote_C->type, T->cote_D->type, T->cote_A->type, T->cote_E->type, T->id_tuile);
    return T;
}




int pose_meeple (Tuile **P, int ligne, int colonne, int num_meeple, int cote)
{
    int pose = 0;
    switch (cote)
    {
    case 1:
        if (P[ligne][colonne].cote_A->cotient_meeple == 1)
        {
            printf("Il y a déjà un meeple\n");
            pose = 0;
        }
        else
        {
            P[ligne][colonne].cote_A->num_meeple = num_meeple;
            
            P[ligne][colonne].cote_A->cotient_meeple = 1;

            pose = 1;
        }
        break;

    case 2:
        if (P[ligne][colonne].cote_B->cotient_meeple == 1)
        {
            printf("Il y a déjà un meeple\n");
            pose = 0;
        }
        else
        {
            P[ligne][colonne].cote_B->num_meeple = num_meeple;;
            P[ligne][colonne].cote_B->cotient_meeple = 1;

            pose = 1;
        }
        break;

    case 3:
        if (P[ligne][colonne].cote_C->cotient_meeple == 1)
        {
            printf("Il y a déjà un meeple\n");
            pose = 0;
        }
        else
        {
            P[ligne][colonne].cote_C->num_meeple = num_meeple;
            P[ligne][colonne].cote_C->cotient_meeple = 1;

            pose = 1;
        }
        break;
    
    case 4:
        if (P[ligne][colonne].cote_D->cotient_meeple == 1)
        {
            printf("Il y a déjà un meeple\n");
            pose = 0;
        }
        else
        {
            P[ligne][colonne].cote_D->num_meeple = num_meeple;
            P[ligne][colonne].cote_D->cotient_meeple = 1;

            pose = 1;
        }
        break;
    
    case 5:
        if (P[ligne][colonne].cote_E->cotient_meeple == 1)
        {
            printf("Il y a déjà un meeple\n");
            pose = 0;
        }
        else
        {
            P[ligne][colonne].cote_E->num_meeple = num_meeple;
            P[ligne][colonne].cote_E->cotient_meeple = 1;

            pose = 1;
        }
        break;
    default:
        pose = 0;
        break;
    }

    if (P[ligne][colonne].cote_E->type == P[ligne][colonne].cote_A->type )
    {
        if (P[ligne][colonne].cote_E->cotient_meeple == 1 || P[ligne][colonne].cote_A->cotient_meeple == 1)
        {
            P[ligne][colonne].cote_E->cotient_meeple = 1;
            P[ligne][colonne].cote_A->cotient_meeple = 1;
            P[ligne][colonne].cote_E->num_meeple = num_meeple;
            P[ligne][colonne].cote_A->cotient_meeple = num_meeple;
        }
    }

    if ( P[ligne][colonne].cote_E->type == P[ligne][colonne].cote_B->type)
    {
        if (P[ligne][colonne].cote_E->cotient_meeple == 1 || P[ligne][colonne].cote_B->cotient_meeple == 1)
        {
            P[ligne][colonne].cote_E->cotient_meeple = 1;
            P[ligne][colonne].cote_B->cotient_meeple = 1;
            P[ligne][colonne].cote_E->num_meeple = num_meeple;
            P[ligne][colonne].cote_B->cotient_meeple = num_meeple;
        }
    }

    if ( P[ligne][colonne].cote_E->type == P[ligne][colonne].cote_C->type )
    {
        if (P[ligne][colonne].cote_E->cotient_meeple == 1 || P[ligne][colonne].cote_C->cotient_meeple == 1)
        {
            P[ligne][colonne].cote_E->cotient_meeple = 1;
            P[ligne][colonne].cote_C->cotient_meeple = 1;
            P[ligne][colonne].cote_E->num_meeple = num_meeple;
            P[ligne][colonne].cote_C->cotient_meeple = num_meeple;
        }
    }

    if ( P[ligne][colonne].cote_E->type == P[ligne][colonne].cote_D->type )
    {
        if (P[ligne][colonne].cote_E->cotient_meeple == 1 || P[ligne][colonne].cote_D->cotient_meeple == 1)
        {
            P[ligne][colonne].cote_E->cotient_meeple = 1;
            P[ligne][colonne].cote_D->cotient_meeple = 1;
            P[ligne][colonne].cote_E->num_meeple = num_meeple;
            P[ligne][colonne].cote_D->cotient_meeple = num_meeple;
        }
    }
    

    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        for (int j=0 ; j<NB_TUILE_MAX ; j++)
        {
            if (P[i][j].vide == 0 )
            {
                if (P[i-1][j].cote_C->cotient_meeple == 1 || P[i][j].cote_A->cotient_meeple == 1)
                {
                    P[i-1][j].cote_C->cotient_meeple = 1;
                    P[i][j].cote_A->cotient_meeple = 1;
                }

                if (P[i+1][j].cote_A->cotient_meeple == 1 || P[i][j].cote_C->cotient_meeple == 1)
                {
                    P[i+1][j].cote_A->cotient_meeple = 1;
                    P[i][j].cote_C->cotient_meeple = 1;
                }

                if (P[i][j-1].cote_B->cotient_meeple == 1 || P[i][j].cote_D->cotient_meeple == 1)
                {
                    P[i][j-1].cote_B->cotient_meeple = 1;
                    P[i][j].cote_D->cotient_meeple = 1;
                }

                if (P[i][j+1].cote_D->cotient_meeple == 1 || P[i][j].cote_B->cotient_meeple == 1)
                {
                    P[i][j+1].cote_D->cotient_meeple = 1;
                    P[i][j].cote_B->cotient_meeple = 1;
                }
            }
        }
    }
    

    return pose;
}