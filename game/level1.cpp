#include "level1.h"
#include <SDL2/SDL_image.h>

void level1Loop(SDL_Renderer* renderer)
{
    SDL_Texture* fondo = IMG_LoadTexture(renderer, "game/assets/fondo_nivel1.png");
    SDL_Texture* mono  = IMG_LoadTexture(renderer, "game/assets/mono.png");
    SDL_Texture* leon  = IMG_LoadTexture(renderer, "game/assets/leon.png");
    SDL_Texture* aro   = IMG_LoadTexture(renderer, "game/assets/aro.png");

    SDL_Event e;
    bool running = true;

    SDL_Rect monoRect = { 50, 400, 80, 80 };

    while (running)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) running = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, fondo, NULL, NULL);
        SDL_RenderCopy(renderer, mono, NULL, &monoRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(fondo);
    SDL_DestroyTexture(mono);
    SDL_DestroyTexture(leon);
    SDL_DestroyTexture(aro);
}
