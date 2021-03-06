#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"

#define color(param) printf("\033[%sm",param)

//  30 Noir |31 Rouge | 32 Vert | 33 Jaune | 34 Bleu| 35 Magenta | 36 Cyan | 37 Blanc
char *tab_color[6] = {"0", "30", "31", "32", "34", "35"};

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

            plateau[i][j].cote_A->cotient_meeple = 0;
            plateau[i][j].cote_B->cotient_meeple = 0;
            plateau[i][j].cote_C->cotient_meeple = 0;
            plateau[i][j].cote_D->cotient_meeple = 0;
            plateau[i][j].cote_E->cotient_meeple = 0;
            
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


int pose_tuile (Tuile** plateau, Tuile *T, int ligne, int colonne )  //ligne et colonne sont les coordonnées de la case ounon va poser la tuile sur le plateau
{
    int i, j;
    if (T == NULL)
    {
        printf ("Tuile inexistante\n");
        return 0;
    }
    if (plateau[ligne][colonne].vide == 1 && plateau[ligne][colonne].available == 1)
    {   
        plateau[ligne][colonne].cote_A->type = T->cote_A->type;
        plateau[ligne][colonne].cote_B->type = T->cote_B->type;
        plateau[ligne][colonne].cote_C->type = T->cote_C->type;
        plateau[ligne][colonne].cote_D->type = T->cote_D->type;
        plateau[ligne][colonne].cote_E->type = T->cote_E->type;
        plateau[ligne][colonne].vide = 0;


        for (i=0 ; i<NB_TUILE_MAX ; i++)
        {
            for (j=0 ; j<NB_TUILE_MAX ; j++)
            {
                plateau[i][j].available = 0;
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
                if (plateau[i][j].cote_A->cotient_meeple == 1)
                {
                    color (tab_color[plateau[i][j].cote_A->num_meeple]);
                    printf ("  %c  ", plateau[i][j].cote_A->type);
                    color (tab_color[0]);
                }
                else
                    printf ("  %c  ", plateau[i][j].cote_A->type);
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
                if (plateau[i][j].cote_D->cotient_meeple == 1)
                {
                    color (tab_color[plateau[i][j].cote_D->num_meeple]);
                    printf ("%c", plateau[i][j].cote_D->type);
                    color (tab_color[0]);
                }
                else
                    printf ("%c", plateau[i][j].cote_D->type);
                
                if (plateau[i][j].cote_E->cotient_meeple == 1)
                {
                    color (tab_color[plateau[i][j].cote_E->num_meeple]);
                    printf (" %c ", plateau[i][j].cote_E->type);
                    color (tab_color[0]);
                }
                else
                    printf (" %c ", plateau[i][j].cote_E->type);
                

                if (plateau[i][j].cote_B->cotient_meeple == 1)
                {
                    color (tab_color[plateau[i][j].cote_B->num_meeple]);
                    printf ("%c", plateau[i][j].cote_B->type);
                    color (tab_color[0]);
                }
                else
                    printf ("%c", plateau[i][j].cote_B->type);

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
                printf ("|");
                if (plateau[i][j].cote_C->cotient_meeple == 1)
                {
                    color (tab_color[plateau[i][j].cote_C->num_meeple]);
                    printf ("  %c  ", plateau[i][j].cote_C->type);
                    color (tab_color[0]);
                }
                else
                    printf ("  %c  ", plateau[i][j].cote_C->type);

            }
        }
        printf ("\n");
    }
    printf ("%s", border);
    printf ("\n");
}

int update_plateau (Tuile** plateau, Tuile *T)
{
    Tuile *temp = T;
    int i, j, posable = 0, nb_rotate = 0;
    for (i=0 ; i<72 ; i++)
    {
        for (j=0 ; j<72 ; j++)
        {
            if (plateau[i][j].vide == 0 )
            {   
                if (i-1 >= 0 && j-1 >= 0 && j+1 <= NB_TUILE_MAX && i-2 >=0 && plateau[i-1][j].vide == 1 )
                {
                    while (plateau[i][j].cote_A->type != temp->cote_C->type  && nb_rotate <4)
                    {
                        temp = rotate_tuile (temp);
                        nb_rotate++;
                    }
                    
                    if (plateau[i-1][j-1].vide == 0 && plateau[i-1][j+1].vide == 0 && plateau[i-2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-2][j].cote_C->type == temp->cote_A->type && plateau[i-1][j+1].cote_D->type == temp->cote_B->type && plateau[i-1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i-1][j+1].vide == 0 && plateau[i-2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-2][j].cote_C->type == temp->cote_A->type && plateau[i-1][j+1].cote_D->type == temp->cote_B->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 0 && plateau[i-1][j+1].vide == 1 && plateau[i-2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-2][j].cote_C->type == temp->cote_A->type && plateau[i-1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 0 && plateau[i-1][j+1].vide == 0 && plateau[i-2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-1][j+1].cote_D->type == temp->cote_B->type && plateau[i-1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i-1][j+1].vide == 1 && plateau[i-2][j].vide == 0)
                    {
                        if ( plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-2][j].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i-1][j+1].vide == 0 && plateau[i-2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-1][j+1].cote_D->type == temp->cote_B->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 0 && plateau[i-1][j+1].vide == 1 && plateau[i-2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type && plateau[i-1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i-1][j+1].vide == 1 && plateau[i-2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_A->type == temp->cote_C->type)
                        {
                            plateau[i-1][j].available = 1;
                            posable = 1;

                        }
                    }
                }
    /*---------------------------------------------------------------------------------------------------------------------------*/
                temp = T;
                nb_rotate = 0;


                if (i+1 <= NB_TUILE_MAX && j-1 >= 0 && j+1 <= NB_TUILE_MAX && i+2 <=NB_TUILE_MAX && plateau[i+1][j].vide == 1 )
                {
                    while (plateau[i][j].cote_C->type != temp->cote_A->type && nb_rotate <4)
                    {
                        temp = rotate_tuile (temp);
                        nb_rotate++;
                    }
                    
                    if (plateau[i+1][j-1].vide == 0 && plateau[i+1][j+1].vide == 0 && plateau[i+2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+2][j].cote_A->type == temp->cote_C->type && plateau[i+1][j+1].cote_D->type == temp->cote_B->type && plateau[i+1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 1 && plateau[i+1][j+1].vide == 0 && plateau[i+2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+2][j].cote_A->type == temp->cote_C->type && plateau[i+1][j+1].cote_D->type == temp->cote_B->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 0 && plateau[i+1][j+1].vide == 1 && plateau[i+2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+2][j].cote_A->type == temp->cote_C->type && plateau[i+1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 0 && plateau[i+1][j+1].vide == 0 && plateau[i+2][j].vide == 1)
                    {
                        if ( plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+1][j+1].cote_D->type == temp->cote_B->type && plateau[i+1][j-1].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 1 && plateau[i+1][j+1].vide == 1 && plateau[i+2][j].vide == 0)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+2][j].cote_A->type == temp->cote_C->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 1 && plateau[i+1][j+1].vide == 0 && plateau[i+2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+1][j+1].cote_D->type == temp->cote_B->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 0 && plateau[i+1][j+1].vide == 1 && plateau[i+2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type && plateau[i+1][j-1].cote_B->type == temp->cote_D->type )
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i+1][j-1].vide == 1 && plateau[i+1][j+1].vide == 1 && plateau[i+2][j].vide == 1)
                    {
                        if (plateau[i][j].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i+1][j].available = 1;
                            posable = 1;

                        }
                    }
                }

    /*---------------------------------------------------------------------------------------------------------------*/            

                temp = T;
                nb_rotate = 0;


                if (j-1 >= 0 && i-1 >= 0 && i+1 <= NB_TUILE_MAX && j-2 >=0 && plateau[i][j-1].vide == 1 )
                {
                    while ( plateau[i][j].cote_D->type != temp->cote_B->type && nb_rotate <4)
                    {
                        temp = rotate_tuile (temp);
                        nb_rotate++;
                    }
                    
                    if (plateau[i-1][j-1].vide == 0 && plateau[i+1][j-1].vide == 0 && plateau[i][j-2].vide == 0)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type && plateau[i][j-2].cote_B->type == temp->cote_D->type && plateau[i+1][j-1].cote_A->type == temp->cote_C->type && plateau[i-1][j-1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }


                    if (plateau[i-1][j-1].vide == 1 && plateau[i+1][j-1].vide == 0 && plateau[i][j-2].vide == 0)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type && plateau[i][j-2].cote_B->type == temp->cote_D->type && plateau[i+1][j-1].cote_A->type == temp->cote_C->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 0 && plateau[i+1][j-1].vide == 1 && plateau[i][j-2].vide == 0)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type && plateau[i][j-2].cote_B->type == temp->cote_D->type && plateau[i-1][j-1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 0 && plateau[i+1][j-1].vide == 0 && plateau[i][j-2].vide == 1)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type  && plateau[i-1][j-1].cote_A->type ==  temp->cote_C->type && plateau[i+1][j-1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i+1][j-1].vide == 1 && plateau[i][j-2].vide == 0)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type && plateau[i][j-2].cote_B->type == temp->cote_D->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i+1][j-1].vide == 0 && plateau[i][j-2].vide == 1)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type && plateau[i+1][j-1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }
                    
                    if (plateau[i-1][j-1].vide == 0 && plateau[i+1][j-1].vide == 1 && plateau[i][j-2].vide == 1)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type && plateau[i+1][j-1].cote_A->type == temp->cote_C->type && plateau[i-1][j-1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j-1].vide == 1 && plateau[i+1][j-1].vide == 1 && plateau[i][j-2].vide == 1)
                    {
                        if (plateau[i][j].cote_D->type == temp->cote_B->type )
                        {
                            plateau[i][j-1].available = 1;
                            posable = 1;

                        }
                    }

                }



    /*---------------------------------------------------------------------------------------------------------------*/            

                temp = T;
                nb_rotate = 0;


                if (j+1 <= NB_TUILE_MAX && i-1 >= 0 && i+1 <= NB_TUILE_MAX && j+2 <=NB_TUILE_MAX && plateau[i][j+1].vide == 1 )
                {
                    while (plateau[i][j].cote_B->type != temp->cote_D->type && nb_rotate <4)
                    {
                        temp = rotate_tuile (temp);
                        nb_rotate++;
                    }

                    if (plateau[i-1][j+1].vide == 0 && plateau[i+1][j+1].vide == 0 && plateau[i][j+2].vide == 0)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type && plateau[i][j+2].cote_D->type == temp->cote_B->type && plateau[i+1][j+1].cote_A->type == temp->cote_C->type && plateau[i-1][j+1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }
                    
                    if (plateau[i-1][j+1].vide == 1 && plateau[i+1][j+1].vide == 0 && plateau[i][j+2].vide == 0)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type  && plateau[i][j+2].cote_D->type == temp->cote_B->type && plateau[i+1][j+1].cote_A->type == temp->cote_C->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }
                    
                    if (plateau[i-1][j+1].vide == 0 && plateau[i+1][j+1].vide == 1 && plateau[i][j+2].vide == 0)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type  && plateau[i][j+2].cote_D->type == temp->cote_B->type && plateau[i-1][j+1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j+1].vide == 0 && plateau[i+1][j+1].vide == 0 && plateau[i][j+2].vide == 1)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type && plateau[i+1][j+1].cote_A->type == temp->cote_C->type && plateau[i-1][j+1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j+1].vide == 1 && plateau[i+1][j+1].vide == 1 && plateau[i][j+2].vide == 0)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type && plateau[i][j+2].cote_D->type == temp->cote_B->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j+1].vide == 1 && plateau[i+1][j+1].vide == 0 && plateau[i][j+2].vide == 1)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type && plateau[i+1][j+1].cote_A->type == temp->cote_C->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j+1].vide == 0 && plateau[i+1][j+1].vide == 1 && plateau[i][j+2].vide == 1)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type  && plateau[i-1][j+1].cote_C->type == temp->cote_A->type)
                        {
                            plateau[i][j+1].available = 1;
                            posable = 1;

                        }
                    }

                    if (plateau[i-1][j+1].vide == 1 && plateau[i+1][j+1].vide == 1 && plateau[i][j+2].vide == 1)
                    {
                        if (plateau[i][j].cote_B->type == temp->cote_D->type)
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

