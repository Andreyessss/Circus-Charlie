#include "menu.h"
#include <SDL2/SDL_image.h>

void menuLoop(SDL_Renderer* renderer)
{
    SDL_Texture* fondo = IMG_LoadTexture(renderer, "game/assets/fondo_menu.png");

    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN) running = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, fondo, NULL, NULL);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(fondo);
}
