

// commande exécution Unix : gcc main.c -o main $(sdl2-config --cflags --libs)

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WINDOW_WIDTH  1360
#define WINDOW_HEIGHT 650

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
    for (int i=0 ; i<30 ; i++)
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



int main (int argc, char** argv)
{

        int i, j, x, y, nb_image = 0;
        int L = 20;
        SDL_bool programme_launched = SDL_TRUE;

/*-----------------------------Initialisation--------------------------------- */
    if (SDL_Init (SDL_INIT_VIDEO) < 0)
        SDL_ExitWithError ("SDL_Init ehouée");
    
    char** tab_images = get_images_from_files ("src/tiles.txt");
    
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
    SDL_Rect plateau_rects[30][30];

    SDL_Rect pioche;
    pioche.w = pioche.h = 100;
    pioche.x = WINDOW_WIDTH - 100 - 10;
    pioche.y = 10;

    SDL_Rect table;
    table.x = table.y = 0;
    table.w = WINDOW_WIDTH - pioche.w - 20;
    table.h = WINDOW_HEIGHT;

    

    for (i=0 ; i<30 ; i++)
    {
        for (j=0 ; j<30 ; j++)
        {
            plateau_rects[i][j].w = L;
            plateau_rects[i][j].h = L;
            if (i<15)
                plateau_rects[i][j].x = (table.w/2) - ((15-i)*L);
            else
                plateau_rects[i][j].x = (table.w/2) + ((i-15)*L);
            if (j<15)
                plateau_rects[i][j].y = (WINDOW_HEIGHT/2) - ((15-j)*L);
            else
                plateau_rects[i][j].y = (WINDOW_HEIGHT/2) + ((j-15)*L);

        }
    }

    

    SDL_Texture *plateau_textures [30][30];
    SDL_Texture* pioche_text, *table_text;
/*--------------------------------traitements ---------------------------------*/

    table_text = create_texture (renderer, "src/table.png");
    pioche_text = create_texture (renderer, tab_images[nb_image]);

    while (programme_launched && nb_image <31 )
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
                if (event.button.x < plateau_rects[29][29].x+L && event.button.y < plateau_rects[29][29].y+L)
                {
                    x = (event.button.x - plateau_rects[0][0].x)/L;
                    y = (event.button.y - plateau_rects[0][0].y)/L;
                    printf ("(%d, %d, %d, %d)\n", x, y, event.button.x, event.button.y);
                    plateau_textures[x][y] = create_texture (renderer, tab_images[nb_image]);
                    nb_image++;
                    if (nb_image >= 30 )
                        pioche_text = create_texture (renderer, "src/table.png");
                    else
                        pioche_text = create_texture (renderer, tab_images[nb_image]);
                }
                break;
            
            case SDL_MOUSEMOTION:
                //printf ("(%d, %d)\n", event.motion.x, event.motion.y);

                break;
            
            case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0 && L < 100)
                    {
                        L += 5;
                        for (i=0 ; i<30 ; i++)
                        {
                            for (j=0 ; j<30 ; j++)
                            {
                                plateau_rects[i][j].w = L;
                                plateau_rects[i][j].h = L;
                                if (i<15)
                                    plateau_rects[i][j].x = (table.w/2) - ((15-i)*L);
                                else
                                    plateau_rects[i][j].x = (table.w/2) + ((i-15)*L);
                                if (j<15)
                                    plateau_rects[i][j].y = (WINDOW_HEIGHT/2) - ((15-j)*L);
                                else
                                    plateau_rects[i][j].y = (WINDOW_HEIGHT/2) + ((j-15)*L);
                            }
                        }
                    }

                    if (event.wheel.y < 0 &&  L > 10 )
                    {
                        L -= 5;
                        for (int i=0 ; i<30 ; i++)
                        {
                            for (int j=0 ; j<30 ; j++)
                            {
                                plateau_rects[i][j].w = L;
                                plateau_rects[i][j].h = L;
                                if (i<15)
                                    plateau_rects[i][j].x = (table.w/2) - ((15-i)*L);
                                else
                                    plateau_rects[i][j].x = (table.w/2) + ((i-15)*L);
                                if (j<15)
                                    plateau_rects[i][j].y = (WINDOW_HEIGHT/2) - ((15-j)*L);
                                else
                                    plateau_rects[i][j].y = (WINDOW_HEIGHT/2) + ((j-15)*L);
                            }
                        }
                    }
                    break;
            
            default:
                break;
            }
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

        SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255); // On choisis la couleur rouge 
        SDL_RenderDrawRect (renderer, &pioche);
        SDL_RenderCopy(renderer, table_text, NULL, &table);
        for (i = 0 ; i<30 ; i++)
        {
            for (j=0 ; j<30 ; j++)
            {
                SDL_RenderDrawRect (renderer, &plateau_rects[i][j]);
                SDL_RenderCopy(renderer, plateau_textures[i][j], NULL, &plateau_rects[i][j]);
            }

        }
        SDL_RenderCopy(renderer, pioche_text, NULL, &pioche);
            
        SDL_RenderPresent (renderer);
        
    }
    

    

/*--------------------destruction et libération de la mémoire-------------------*/

    for ( i = 0; i < 30; i++)
        SDL_DestroyTexture (plateau_textures[i][j]);
    
    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    free_tab_images (tab_images);
    SDL_Quit ();
    return 0;
}



/*
int main(int argc, char* argv[])
{


    char** tab_images = get_images_from_files ("src/tiles.txt");

    for (int i=0 ; i<10 ; i++)
        printf ("ilage %s\n", tab_images[i]);


    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERreur SDL_Init : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* window = SDL_CreateWindow ("Carcassonne", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer (window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s \n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Surface *image, *image_pioche, *image_p;
    SDL_Texture *texture, *texture_p;
    SDL_Texture *tab_texture[10];

    SDL_bool programme_launched = SDL_TRUE;

    SDL_Rect Pioche;
    Pioche.h = 100;
    Pioche.w = 100;
    Pioche.x = WINDOW_WIDTH - Pioche.w - 10;
    Pioche.y = 10;

    SDL_Rect Rect_background;
    Rect_background.w = WINDOW_WIDTH - Pioche.w - 20;
    Rect_background.h = WINDOW_HEIGHT;
    Rect_background.x = 0;
    Rect_background.y = 0;

    int L = 18;
    

    SDL_Rect tab_plateau [72][72];

    for (int i=0 ; i< 72 ; i++)
    {
        for (int j=0 ; j<72 ; j++)
        {
            tab_plateau[i][j].x = i * L;
            tab_plateau[i][j].y = j * L;
            tab_plateau[i][j].h = L;
            tab_plateau[i][j].w = L;

        }
    }

   int nb_image = 0;

   image = SDL_LoadBMP ("src/table.bmp");
                        
    if (image == NULL)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        fprintf(stderr, "Erreur SDL_LoadBMP : %s \n", SDL_GetError());
        return EXIT_FAILURE;
    }

    texture = SDL_CreateTextureFromSurface (renderer, image);
    if (texture == NULL)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s \n", SDL_GetError());
        return EXIT_FAILURE;
                            
    }
    image_pioche = SDL_LoadBMP ("src/table.bmp");
                        
    if (image_pioche == NULL)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        fprintf(stderr, "Erreur SDL_LoadBMP : %s \n", SDL_GetError());
        return EXIT_FAILURE;
    }

    tab_texture[nb_image] = SDL_CreateTextureFromSurface (renderer, image_pioche);
    if (tab_texture[nb_image] == NULL)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s \n", SDL_GetError());
        return EXIT_FAILURE;
                            
    }

    SDL_FreeSurface (image);
    SDL_FreeSurface (image_pioche);
    int x, y;

    while (programme_launched)
    {

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    programme_launched = SDL_FALSE;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if ( (event.button.x > Pioche.x && event.button.x < Pioche.x + Pioche.w) && (event.button.y > Pioche.y && event.button.y < Pioche.y + Pioche.h))
                    {
                        nb_image++;
                        image = SDL_LoadBMP (tab_images[nb_image]);
                        
                        if (image == NULL)
                        {
                            SDL_DestroyRenderer (renderer);
                            SDL_DestroyWindow (window);
                            fprintf(stderr, "Erreur SDL_LoadBMP : %s \n", SDL_GetError());
                            return EXIT_FAILURE;
                        }

                        tab_texture[nb_image] = SDL_CreateTextureFromSurface (renderer, image);
                        if (tab_texture[nb_image] == NULL)
                        {
                            SDL_DestroyRenderer (renderer);
                            SDL_DestroyWindow (window);
                            fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s \n", SDL_GetError());
                            return EXIT_FAILURE;
                            
                        }

                        SDL_FreeSurface (image);
                    }
                    else 
                    {
                        x = event.button.x/L;
                        y = event.button.y/L;
                        image_p = SDL_LoadBMP (tab_images[nb_image]);
                        if (image_p == NULL)
                        {
                            SDL_DestroyRenderer (renderer);
                            SDL_DestroyWindow (window);
                            fprintf(stderr, "Erreur SDL_LoadBMP : %s \n", SDL_GetError());
                            return EXIT_FAILURE;
                        }

                        
                        tab_texture[nb_image] = SDL_CreateTextureFromSurface (renderer, image_p);
                        if (tab_texture[nb_image] == NULL)
                        {
                            SDL_DestroyRenderer (renderer);
                            SDL_DestroyWindow (window);
                            fprintf(stderr, "Erreur SDL_CreateTextureFromSurface : %s \n", SDL_GetError());
                            return EXIT_FAILURE;
                                                
                        }
                        SDL_FreeSurface (image_p);
                    }

                    break;

                case SDL_MOUSEWHEEL:
                    if (event.wheel.y > 0 && L < 75)
                    {
                        L += 5;
                        for (int i=0 ; i<72 ; i++)
                        {
                            for (int j=0 ; j<72 ; j++)
                            {
                                tab_plateau[i][j].x = i * L;
                                tab_plateau[i][j].y = j * L;
                                tab_plateau[i][j].h = L;
                                tab_plateau[i][j].w = L;
                            }
                        }
                    }

                    if (event.wheel.y < 0 &&  L > 15 )
                    {
                        L -= 5;
                        for (int i=0 ; i<72 ; i++)
                        {
                            for (int j=0 ; j<72 ; j++)
                            {
                                tab_plateau[i][j].x = i * L;
                                tab_plateau[i][j].y = j * L;
                                tab_plateau[i][j].h = L;
                                tab_plateau[i][j].w = L;
                            }
                        }
                    }
                    break;

                default:
                    break;
            }
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
        
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // On choisis la couleur rouge 
        SDL_RenderDrawRect (renderer, &Pioche);
        SDL_RenderDrawLine (renderer, WINDOW_WIDTH - 200, 0, WINDOW_WIDTH - 200, WINDOW_HEIGHT);
        SDL_RenderCopy(renderer, texture, NULL, &Rect_background);
        SDL_RenderCopy(renderer, tab_texture[nb_image], NULL, &Pioche);
        SDL_RenderCopy(renderer, texture_p, NULL, &tab_plateau[x][y]);
        for (int i = 0 ; i<72 ; i++)
            for (int j=0 ; j<72 ; j++)
            {
                SDL_RenderDrawRect (renderer, &tab_plateau[i][j]);
                SDL_RenderCopy(renderer, tab_texture[nb_image], NULL, &tab_plateau[x][y]);

            }
        
        SDL_RenderPresent (renderer);

    }

    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Quit();

    free_tab_images (tab_images);
    
    return 0;
}
*/