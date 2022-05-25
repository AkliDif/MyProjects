#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Pile.h"
#include "Plateau.h"
#include "Player.h"
#include "Tuile.h"

#define WINDOW_WIDTH  1360
#define WINDOW_HEIGHT 650
#define NB_IMAGES 30

char** get_images_from_files (char* file)
{
    FILE* f = fopen (file, "r");

    if (f == NULL)
    {
        fprintf (stderr, "Erreur fopen : erruer d'ouvrture de fichier %s \n", file);
        exit(1);
    }
    char c =(char) fgetc (f);
    int ligne = 1;
    while (  c != EOF)
    {
        if (c == '\n')
            ligne++;
        c =(char) fgetc (f);
    }

    char** tab_images = (char**)malloc (sizeof(char*)*ligne);
    for (int i=0 ; i<ligne ; i++)
        tab_images[i] = (char*)malloc (sizeof(char)*7);
    
    fseek (f, 0, SEEK_SET);
    ligne = 0;

    while (!feof (f))
    {
        fscanf (f, "%s", tab_images[ligne]);
        ligne++;
    }
    

    fclose (f);
    return tab_images;
}

void free_tab_images (char** tab_image)
{
    for (int i=0 ; i<NB_IMAGES ; i++)
        free(tab_image[i]);
    free (tab_image);
}

SDL_Texture* create_texture (SDL_Renderer* renderer,  char* file)
{
    SDL_Texture *texture;
    SDL_Surface *image = IMG_Load (file);
                        
    if (image == NULL)
    {
        fprintf(stderr, "Erreur IMG_Load : %s \n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    texture = SDL_CreateTextureFromSurface (renderer, image);
    if (texture == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s \n", SDL_GetError());
        exit(EXIT_FAILURE);         
    }
    
    SDL_FreeSurface (image);

    return texture;
}
void SDL_ExitWithError (const char* message)
{
    SDL_Log ("Erreur : initialisation SDL > %s \n", message);
    SDL_Quit ();
    exit (EXIT_FAILURE);
}



int main (void)
{
    int i, j, x, y, nb_image = 1, nb_player;
    int L = 20;
    SDL_bool programme_launched = SDL_TRUE;
    
    int ligne, colonne, point = 0;
    
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
    
    Pile* P = creer_pile ();

    
    Tuile** Plateau = creer_plateau ();


    

    Tuile *temp;

    Pile* P_temp = init_pile ();
    int num_player = 1;


    /*-----------------------------Initialisation--------------------------------- */
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
        SDL_ExitWithError ("SDL_Init ehouée");
    
    char** tab_images = get_images_from_files ("tiles_images.txt");
    
    if (tab_images == NULL)
        SDL_ExitWithError ("Recuperation images echouée");
    
    SDL_Window* window = SDL_CreateWindow ("Carcassonne", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);

    if (window == NULL)
        SDL_ExitWithError ("creation de fenetre echouée");
    
    SDL_Renderer* renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        SDL_ExitWithError ("CReation de renderer echouée");
        SDL_DestroyWindow (window);
    }
    SDL_Rect plateau_rects[NB_TUILE_MAX][NB_TUILE_MAX];

    SDL_Rect pioche_rect;
    pioche_rect.w = pioche_rect.h = 100;
    pioche_rect.x = WINDOW_WIDTH - 100 - 10;
    pioche_rect.y = 10;

    SDL_Rect table;
    table.x = table.y = 0;
    table.w = WINDOW_WIDTH - pioche_rect.w - 20;
    table.h = WINDOW_HEIGHT;

    

    for (i=0 ; i<NB_TUILE_MAX ; i++)
    {
        for (j=0 ; j<NB_TUILE_MAX ; j++)
        {
            plateau_rects[i][j].w = L;
            plateau_rects[i][j].h = L;
            if (i<NB_TUILE_MAX/2)
                plateau_rects[i][j].x = (table.w/2) - (((NB_TUILE_MAX/2)-i)*L);
            else
                plateau_rects[i][j].x = (table.w/2) + ((i-(NB_TUILE_MAX/2))*L);
            if (j<NB_TUILE_MAX/2)
                plateau_rects[i][j].y = (WINDOW_HEIGHT/2) - (((NB_TUILE_MAX/2)-j)*L);
            else
                plateau_rects[i][j].y = (WINDOW_HEIGHT/2) + ((j-(NB_TUILE_MAX/2))*L);

        }
    }

    

    SDL_Texture *plateau_textures [NB_TUILE_MAX][NB_TUILE_MAX];
    SDL_Texture* pioche_text, *table_text;
/*--------------------------------traitements ---------------------------------*/

    table_text = create_texture (renderer, "./img/table.png");
    pioche_text = create_texture (renderer, tab_images[nb_image]);

    Tuile* pioche;
    
    pioche = depiler_tuile (P);
    print_tile (pioche);
    
    if (pose_tuile (Plateau, pioche, 35, 35) != 1)
    {
        fprintf(stderr, "\nTuile de départ non déposé\n");
        exit (EXIT_FAILURE);
    }

    
    plateau_textures[35][35] = create_texture (renderer, tab_images[0]);
    free_tile (pioche);
    pioche = depiler_tuile (P);


    while (update_plateau (Plateau, pioche)!= 1)
    {
        empiler_tuile(P_temp, *pioche);
        pioche = depiler_tuile(P);
    }

    print_tile(pioche);

    pioche_text = create_texture (renderer, tab_images[pioche->id_tuile]);

        while (programme_launched)
        {
            SDL_Event event;
            

            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        programme_launched = false;
                        break;
                    
                    case SDL_MOUSEBUTTONDOWN:
                        if ((event.button.x < plateau_rects[NB_TUILE_MAX-1][NB_TUILE_MAX-1].x+L && event.button.y < plateau_rects[NB_TUILE_MAX-1][NB_TUILE_MAX-1].y+L)) 
                        {
                            while (P_temp->nb_tuile > 0)
                            {
                                temp = depiler_tuile (P_temp);
                                empiler_tuile (P, *temp);
                                free_tile (temp);
                            }
                            x = (event.button.x - plateau_rects[0][0].x)/L;
                            y = (event.button.y - plateau_rects[0][0].y)/L;
                            printf ("(%d, %d)\n", y, x);
                            if (pose_tuile (Plateau, pioche, y, x) == 1)
                            {

                                plateau_textures[x][y] = pioche_text;
                                free_tile (pioche);
                                pioche= depiler_tuile (P);
                                while (update_plateau (Plateau, pioche)!= 1)
                                {
                                    empiler_tuile(P_temp, *pioche);
                                    pioche = depiler_tuile(P);
                                }
                                pioche_text = create_texture (renderer, tab_images[pioche->id_tuile]);
                                affichage_plateau (Plateau);
                                print_tile(pioche);
                            }
                            else
                                printf ("\n\nEmplacement non valabe\n");
                        }
                        break;
                    
                    case SDL_MOUSEMOTION:
                        //printf ("(%d, %d)\n", event.motion.x, event.motion.y);

                        break;
                    
                    case SDL_MOUSEWHEEL:
                            if (event.wheel.y > 0 && L < 100)
                            {
                                L += 5;
                                for (i=0 ; i<NB_TUILE_MAX ; i++)
                                {
                                    for (j=0 ; j<NB_TUILE_MAX ; j++)
                                    {
                                        plateau_rects[i][j].w = L;
                                        plateau_rects[i][j].h = L;
                                        if (i< NB_TUILE_MAX/2)
                                            plateau_rects[i][j].x = (table.w/2) - ((NB_TUILE_MAX/2-i)*L);
                                        else
                                            plateau_rects[i][j].x = (table.w/2) + ((i-NB_TUILE_MAX/2)*L);
                                        if (j<NB_TUILE_MAX/2)
                                            plateau_rects[i][j].y = (WINDOW_HEIGHT/2) - ((NB_TUILE_MAX/2-j)*L);
                                        else
                                            plateau_rects[i][j].y = (WINDOW_HEIGHT/2) + ((j-NB_TUILE_MAX/2)*L);
                                    }
                                }
                            }

                            if (event.wheel.y < 0 &&  L > 10 )
                            {
                                L -= 5;
                                for (int i=0 ; i<NB_TUILE_MAX ; i++)
                                {
                                    for (int j=0 ; j<NB_TUILE_MAX ; j++)
                                    {
                                        plateau_rects[i][j].w = L;
                                        plateau_rects[i][j].h = L;
                                        if (i<NB_TUILE_MAX/2)
                                            plateau_rects[i][j].x = (table.w/2) - ((NB_TUILE_MAX/2-i)*L);
                                        else
                                            plateau_rects[i][j].x = (table.w/2) + ((i-NB_TUILE_MAX/2)*L);
                                        if (j<NB_TUILE_MAX/2)
                                            plateau_rects[i][j].y = (WINDOW_HEIGHT/2) - ((NB_TUILE_MAX/2-j)*L);
                                        else
                                            plateau_rects[i][j].y = (WINDOW_HEIGHT/2) + ((j-NB_TUILE_MAX/2)*L);
                                    }
                                }
                            }
                        break;

                        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_DOWN)
                            {
                                for (i=0 ; i<NB_TUILE_MAX ; i++)
                                {
                                    for (j=0 ; j<NB_TUILE_MAX ; j++)
                                    {
                                        plateau_rects[i][j].y += 5;
                                    }
                                }
                            }

                            if (event.key.keysym.sym == SDLK_UP)
                            {
                                for (i=0 ; i<NB_TUILE_MAX ; i++)
                                {
                                    for (j=0 ; j<NB_TUILE_MAX ; j++)
                                    {
                                        plateau_rects[i][j].y -= 5;
                                    }
                                }
                            }
                            break;
                    
                    default:
                        break;
                }


                if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) != 0)
                {
                    fprintf(stderr, "Erreur SDL_SetRenderDrawColor : %s\n", SDL_GetError());
                    return EXIT_FAILURE;
                }

                if (SDL_RenderClear (renderer) != 0)
                {
                    fprintf(stderr, "Erreur SDL_RenderClear : %s\n", SDL_GetError());
                    return EXIT_FAILURE;
                }

                SDL_SetRenderDrawColor(renderer, 92,91,99,255); // On choisis la couleur rouge 
                SDL_RenderCopy(renderer, table_text, NULL, &table);
                for (i = 0 ; i<NB_TUILE_MAX ; i++)
                {
                    for (j=0 ; j<NB_TUILE_MAX ; j++)
                    {
                        SDL_RenderCopy(renderer, plateau_textures[i][j], NULL, &plateau_rects[i][j]);
                    }

                }

                SDL_RenderCopy(renderer, pioche_text, NULL, &pioche_rect);
                for (i=0; i<NB_TUILE_MAX; i++)
                {
                    for (j=0 ; j<NB_TUILE_MAX ; j++)
                    {
                        if (Plateau[i][j].available == 1 && Plateau[i][j].vide == 1)
                        {
                            SDL_RenderFillRect(renderer, &plateau_rects[j][i]);
                        }
                    }
                }

                SDL_RenderPresent (renderer);

            }
        
        }
       
        /*
        if (Plateau[ligne][colonne].cote_A->occupe == 0 || Plateau[ligne][colonne].cote_B->occupe == 0 || Plateau[ligne][colonne].cote_C->occupe == 0 || Plateau[ligne][colonne].cote_D->occupe == 0 || Plateau[ligne][colonne].cote_E->occupe == 0)
        {
            //printf ("\nVoulez deposer le")
        }
        */


/* ----------------------- libération de la mémoire ------------------------------ */

    free_plateau (Plateau);

    for (i=0 ; i<nb_player ; i++)
        free (tab_player[i]);
    free_pile (P);
    free_pile(P_temp);
    free (P_temp);
    free (P);

    for ( i = 0; i < 30; i++)
        SDL_DestroyTexture (plateau_textures[i][j]);
    
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    free_tab_images (tab_images);
    SDL_Quit ();
    return 0;
}
