// commande exécution Unix : gcc main.c -o main $(sdl2-config --cflags --libs)

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#define WIDTH 1366
#define HEIGHT 640

void SDL_ExitWithError (const char* message)
{
    SDL_Log ("Erreur : initialisation SDL > %s \n", message);
    SDL_Quit ();
    exit (EXIT_FAILURE);
}

int main (void)
{
    int i;
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;


    //lancement de la SDL

    if (SDL_Init (SDL_INIT_VIDEO) != 0)
        SDL_ExitWithError ("Initalisation echouée");


    //creation window + render

    window = SDL_CreateWindow ("Carcassonne", 0, 0, WIDTH, HEIGHT, 0);
    if ( window == NULL)
        SDL_ExitWithError ("Creatio de fenetre echouée");
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if ( renderer == NULL)
        SDL_ExitWithError ("Creation de rendu echouée");
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    

    SDL_RenderPresent (renderer);


    SDL_Rect rect_src, rect_dest;

    SDL_Surface *image;
    SDL_Texture *texture;

    image = SDL_LoadBMP ("table.bmp");
    
    if (image == NULL)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        SDL_ExitWithError ("Impossible de charger l'image");
    }

    texture = SDL_CreateTextureFromSurface (renderer, image);
    if (texture == NULL)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        SDL_ExitWithError ("Impossible de charger la texture");
    }
    SDL_FreeSurface (image);

    rect_src.h = HEIGHT-100;
    rect_src.w = WIDTH-100;
    rect_src.x = (WIDTH - rect_src.w)/2;
    rect_src.y = (HEIGHT - rect_src.h)/2;


    if (SDL_QueryTexture (texture, NULL, NULL,  &rect_src.w,  &rect_src.h) != 0)
    {
        SDL_DestroyRenderer (renderer);
        SDL_DestroyWindow (window);
        SDL_ExitWithError ("Impossible de charger la texture");
    }


    SDL_RenderCopy(renderer, texture, NULL, &rect_src);
    SDL_RenderPresent (renderer);

    SDL_SetRenderTarget(renderer, texture);

    /*---------------------------------------------------------------------------------------------------------------------*/

    SDL_bool programme_lance = SDL_TRUE;
    int x, y;

    while (programme_lance)
    {
        SDL_Event event;

        while (SDL_PollEvent (&event))
        {
            switch (event.type)
            {
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x / 50;
                    y = event.button.y / 50;
                    printf ("(%d, %d)\n", x, y);
                    break;
                case SDL_MOUSEWHEEL:
                    if (event.wheel.y < 0)
                    {
                        printf ("wheel.y = %d\n", event.wheel.y);
                        rect_src.h -= 5;
                        rect_src.w -= 5;

                        rect_src.x = (WIDTH - rect_src.w)/2;
                        rect_src.y = (HEIGHT - rect_src.h)/2;


                        SDL_RenderCopy (renderer, texture, NULL, &rect_src);
                        SDL_RenderPresent (renderer);
                    }
                    else if (event.wheel.y > 0)
                    {

                        printf ("wheel.y = %d\n", event.wheel.y);
                        rect_src.h += 5;
                        rect_src.w += 5;

                        rect_src.x = (WIDTH - rect_src.w)/2;
                        rect_src.y = (HEIGHT - rect_src.h)/2;

                        SDL_RenderCopy (renderer, texture, NULL, &rect_src);
                        SDL_RenderPresent (renderer);
                    }
                    break;
                case SDL_QUIT:
                    programme_lance = SDL_FALSE;
                    break;
            
                default:
                    break;
            }
        }
        
    }   



    /*---------------------------------------------------------------------------------------------------------------------*/

    SDL_DestroyRenderer (renderer);
    SDL_DestroyWindow (window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
