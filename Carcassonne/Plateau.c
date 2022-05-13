#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Meeple.h"
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"


char tab_color[6][2] = {"47", "40", "41", "42", "43", "44"};


Tuile** creer_plateau ()
{
    Tuile** plateau = (Tuile**)malloc (NB_TUILE_MAX * sizeof(Tuile*));

    if (plateau == NULL)
    {
        fprintf(stderr, "Error : malloc ehoué\n");
        exit (1);
    }
    
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        plateau[i] = (Tuile*)malloc (NB_TUILE_MAX * sizeof(Tuile));
        if (plateau[i] == NULL)
        {
            fprintf(stderr, "Error : malloc ehoué\n");
            exit (1);
        }
    }

    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        for (int j=0; j<NB_TUILE_MAX ; j++)
        {
            plateau[i][j].vide = 1;
            plateau[i][j].available = 0;
            plateau[i][j].cote_A = (Cote*)malloc(sizeof(Cote));
            plateau[i][j].cote_B = (Cote*)malloc(sizeof(Cote));
            plateau[i][j].cote_C = (Cote*)malloc(sizeof(Cote));
            plateau[i][j].cote_D = (Cote*)malloc(sizeof(Cote));
            plateau[i][j].cote_E = (Cote*)malloc(sizeof(Cote));


            plateau[i][j].cote_A->Pion = (Meeple*)malloc(sizeof(Meeple));
            plateau[i][j].cote_B->Pion = (Meeple*)malloc(sizeof(Meeple));
            plateau[i][j].cote_C->Pion = (Meeple*)malloc(sizeof(Meeple));
            plateau[i][j].cote_D->Pion = (Meeple*)malloc(sizeof(Meeple));
            plateau[i][j].cote_E->Pion = (Meeple*)malloc(sizeof(Meeple)); 
            
        }
    }

    plateau[35][35].available = 1;


    return plateau;
}


void free_plateau (Tuile** plateau)
{
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        for (int j=0; j<NB_TUILE_MAX ; j++)
        {
            free (plateau[i][j].cote_A->Pion);
            free (plateau[i][j].cote_B->Pion);
            free (plateau[i][j].cote_C->Pion);
            free (plateau[i][j].cote_D->Pion);
            free (plateau[i][j].cote_E->Pion);


            free (plateau[i][j].cote_A);
            free (plateau[i][j].cote_B);
            free (plateau[i][j].cote_C);
            free (plateau[i][j].cote_D);
            free (plateau[i][j].cote_E);
            
        }
    }
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        free (plateau[i]);
    }

    free(plateau);
}


int pose_tuile (Tuile** plateau, Tuile *T, int ligne, int colonne, int num_player)  //ligne et colonne sont les coordonnées de la case ounon va poser la tuile sur le plateau
{
    int i, j;
    if (T == NULL)
    {
        printf ("Tuile inexistante\n");
        return 0;
    }
    if (plateau[ligne][colonne].vide == 1 && plateau[ligne][colonne].available == 1)
    {
        plateau[ligne][colonne].num_player = num_player;
        strcpy(plateau[ligne][colonne].cote_A->type, T->cote_A->type);
        strcpy(plateau[ligne][colonne].cote_B->type, T->cote_B->type);
        strcpy(plateau[ligne][colonne].cote_C->type, T->cote_C->type);
        strcpy(plateau[ligne][colonne].cote_D->type, T->cote_D->type);
        strcpy(plateau[ligne][colonne].cote_E->type, T->cote_E->type);
        plateau[ligne][colonne].vide = 0;
        plateau[ligne][colonne].nb = T->nb;


        for (i=0 ; i<NB_TUILE_MAX ; i++)
        {
            for (j=0 ; j<NB_TUILE_MAX ; j++)
            {
                if (plateau[i][j].vide == 1 && plateau[i][j].available == 1)
                {
                    plateau[i][j].available = 0;
                }
            }
        }

        return 1;
    }
    else 
    {
        printf ("La tuile ne peux pas étre posé à cette endroit, poser là ailleurs!");
        return 0;
    }

}

void affichage_plateau (Tuile ** plateau)
{
    char mot[] = "+-----";
    char plus[] = "+";
    char border[6*71+1] = "";
    for (int i=0 ; i<71 ; i++)
        strcat (border, mot);
    strcat (border, plus);
    for (int i=0 ; i<NB_TUILE_MAX ; i++)
    {
        printf ("%s\n", border);
        for (int j=0 ; j<NB_TUILE_MAX ; j++)
        {
            if (plateau[i][j].vide == 1)
            {
                if (plateau[i][j].available == 1)
                    printf ("|*****");
                else 
                    printf ("|     ");

            }
            else
            {
                printf ("|");
                printf ("  %s  ", plateau[i][j].cote_A->type);
            }
        }
        printf ("\n");
        for (int j=0 ; j<NB_TUILE_MAX ; j++)
        {
            if (plateau[i][j].vide == 1)
            {
                if (plateau[i][j].available == 1)
                    printf ("|%d, %d", i, j);
                else 
                    printf ("|     ");
            }
            else
            {
                printf ("|");
                printf ("%s ", plateau[i][j].cote_D->type);
                printf ("%s ", plateau[i][j].cote_E->type);
                printf ("%s", plateau[i][j].cote_B->type);
            }
        }
        printf ("\n");

        for (int j=0 ; j<NB_TUILE_MAX ; j++)
        {
            if (plateau[i][j].vide == 1)
            {
                if (plateau[i][j].available == 1)
                    printf ("|*****");
                else 
                    printf ("|     ");
            }
            else
            {
                printf ("|  %s  ", plateau[i][j].cote_C->type);

            }
        }
        printf ("\n");
    }
    printf ("%s", border);
    printf ("\n");
}

int update_plateau (Tuile** plateau, Tuile *T)
{
    int i, j, posable = 0;
    for (i=0 ; i<72 ; i++)
    {
        for (j=0 ; j<72 ; j++)
        {
            if (plateau[i][j].vide == 0)
            {
                if (i-1 >= 0 && j-1 >= 0 && j+1 <= NB_TUILE_MAX && plateau[i-1][j].vide == 1 && plateau[i-1][j-1].vide == 1 && plateau[i-1][j+1].vide == 1)
                {
                    if (i-2 >= 0 && plateau[i-2][j].vide == 1)
                    {
                        if (strcmp (plateau[i][j].cote_A->type, T->cote_C->type) == 0)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }
                }
                if (i+1 <= NB_TUILE_MAX && j-1 >= 0 && j+1 <= NB_TUILE_MAX && plateau[i+1][j].vide == 1 && plateau[i+1][j-1].vide == 1 && plateau[i-1][j+1].vide == 1)
                {
                    if (i+2 <= NB_TUILE_MAX && plateau[i+2][j].vide == 1)
                    {
                        if (strcmp (plateau[i][j].cote_C->type, T->cote_A->type) == 0)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;
                        }
                    }
                }
                if (j-1 >= 0 && i-1 >= 0 && i+1 <= NB_TUILE_MAX && plateau[i][j-1].vide == 1 && plateau[i-1][j-1].vide == 1 && plateau[i+1][j-1].vide == 1)
                {
                    if (j-2 >= 0 && plateau[i][j-2].vide == 1)
                    {
                        if (strcmp (plateau[i][j].cote_D->type, T->cote_B->type) == 0)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;
                        }
                    }
                }
                if (j+1 <= NB_TUILE_MAX && i-1 >= 0 && i+1 <= NB_TUILE_MAX && plateau[i][j+1].vide == 1 && plateau[i+1][j+1].vide == 1 && plateau[i-1][j+1].vide == 1)
                {
                    if (j+2 <= NB_TUILE_MAX && plateau[i][j+2].vide == 1)
                    {
                        if (strcmp (plateau[i][j].cote_B->type, T->cote_D->type) == 0)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;
                        }
                    }
                }
            }
        }
    }

    return posable;

}

int route (Tuile** P, Tuile* T, int ligne, int colonne)
{
    int cpt = 0;
    if (strcmp (T->cote_E->type, "R") != 0)
        return cpt + 1;
    if (strcmp (T->cote_A->type, "R") == 0)
    {
        if ( P[ligne-1][colonne].vide != 1 )
            return route (P, &P[ligne-1][colonne], ligne-1, colonne) + 1 ;
        return cpt;
    }
    if (strcmp (T->cote_C->type, "R") == 0)
    {
        if (P[ligne+1][colonne].vide != 1)
            return route (P, &P[ligne+1][colonne], ligne+1, colonne) + 1;
        return cpt;
    }
    if (strcmp (T->cote_D->type, "R") == 0)
    {
        if (P[ligne][colonne-1].vide != 1)
            return route (P, &P[ligne][colonne-1], ligne, colonne-1) + 1;
        return cpt;
    }
    if (strcmp (T->cote_B->type, "R") == 0)
    {
        if (P[ligne][colonne+1].vide != 1)
            return route (P, &P[ligne][colonne+1], ligne, colonne+1) + 1;
        return cpt;
    }
}