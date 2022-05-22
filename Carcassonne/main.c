#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "Meeple.h"
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"

int main (void)
{
    int nb_player, i, j, ligne, colonne, point = 0, c;
    
    char  rotate;
        
    printf ("\nBienvenue au Jeu Carcasonne :)\n");
    printf ("\nChoisir le nombre de joueurs (2 à 5 joueurs)\n");
    scanf ("%d", &nb_player);
    while (nb_player < 2 || nb_player > 5 )
    {
        printf ("\nNombre de joueurs invalide!\nChoisir le nombre de joueurs (2 à 5 joueurs)\n");
        scanf ("%d", &nb_player);
    }

    Player* tab_player[nb_player];


    for (i=0 ; i< nb_player ; i++)
    {
        tab_player[i] = create_player(i+1);
    }

    Meeple* noire = (Meeple*)malloc(sizeof(Meeple));
    noire->coleur = 1;

    Meeple* rouge = (Meeple*)malloc(sizeof(Meeple));
    rouge->coleur = 2;

    Meeple* vert = (Meeple*)malloc(sizeof(Meeple));
    vert->coleur = 3;

    Meeple* blue = (Meeple*)malloc(sizeof(Meeple));
    blue->coleur = 4;

    Meeple* magenta = (Meeple*)malloc(sizeof(Meeple));
    magenta->coleur = 5;

    Pile* P = creer_pile ();

    
    Tuile** Plateau = creer_plateau ();
    
    Tuile* pioche;
    
    pioche = depiler_tuile (P);
    
    if (pose_tuile (Plateau, pioche, 35, 35, 0) != 1)
    {
        fprintf(stderr, "\nTuile de départ non déposé\n");
        exit (EXIT_FAILURE);
    }

    Tuile *temp;

    Pile* P_temp = init_pile ();
    int num_player = 0;
    

    while (P->nb_tuile > 60)
    {
        num_player = (num_player%nb_player)+1;

        printf ("Player = %d \n", num_player);  
        
        
        pioche = depiler_tuile (P);
        while (P_temp->nb_tuile > 0)
        {
            temp = depiler_tuile (P_temp);
            empiler_tuile (P, *temp);
        }

        if (update_plateau (Plateau, pioche) == 1)
        {

            affichage_plateau (Plateau);
            print_tile (pioche);

            printf("\nOù voulez vous deposer la tuile\nLigne > \n");
            scanf("%d", &ligne);
            printf ("\nColonne >\n");
            scanf ("%d", &colonne);


            while (pose_tuile (Plateau, pioche, ligne, colonne, num_player) != 1)
            {
                printf ("La tuile n'a pas pu etre posée à cette case, rééessayer alleurs!\nLigne >\n");
                scanf("%d", &ligne);
                printf ("\nColonne >\n");
                scanf ("%d", &colonne);
            }


            free_tile (pioche);

        }
        else
        {
            printf ("\nLa tuile n'est pas, posable\n");
            if (empiler_tuile (P_temp, *pioche) == 1)
                printf ("Tuile empiler avec success\n");
            
        
        }

        if (Plateau[ligne][colonne].cote_A->vide == 1 || Plateau[ligne][colonne].cote_B->vide == 1 || Plateau[ligne][colonne].cote_C->vide == 1 || Plateau[ligne][colonne].cote_D->vide == 1 || Plateau[ligne][colonne].cote_E->vide == 1)
        {
            printf ("\nOù voulez deposer le pion\n");
            scanf("%d", &c);

            if (num_player == 1)
                pose_meeple (&Plateau[ligne][colonne], noire, c, num_player);
            if (num_player == 2)
                pose_meeple (&Plateau[ligne][colonne], rouge, c, num_player);
            if (num_player == 3)
                pose_meeple (&Plateau[ligne][colonne], vert, c, num_player);
            if (num_player == 4)
                pose_meeple (&Plateau[ligne][colonne], blue, c, num_player);
            if (num_player == 5)
                pose_meeple (&Plateau[ligne][colonne], magenta, c, num_player);
        }
    }
    


/* ----------------------- libération de la mémoire ------------------------------ */

    free_plateau (Plateau);

    for (i=0 ; i<nb_player ; i++)
        free (tab_player[i]);
    free_pile (P);
    free_pile(P_temp);
    free (P_temp);
    free (P);
    return 0;
}
