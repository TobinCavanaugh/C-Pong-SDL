//
// Created by tobin on 8/5/2023.
//

#include "ScreenTransitions.h"
#include "SharpKeywords.h"

void ScreenWipe(SDL_Window *window, int totalTimeMs, uint32_t color, void (*ScreenCovered)(), void (*AnimComplete)()) {

    //Get the surface
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    //Allocate memory for a copy of the original surface
    SDL_Surface *oldSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32,
                                                   0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

    //Blit the surface to the old surface, taking a screenshot more or less
    SDL_BlitSurface(surface, NULL, oldSurface, NULL);


    //This rect is the one that will create the illusion of the lines sliding down
    SDL_Rect lineRect = {0, surface->h / 2, surface->w, 1};

    int thirdTime = totalTimeMs / 3;

    for (int i = 0; i < surface->h; i++) {
        lineRect.y = i;

        SDL_Delay(thirdTime / surface->h);

        SDL_FillRect(surface, &lineRect, color, 0x00, 0x00);
        SDL_UpdateWindowSurface(window);
    }

    if (ScreenCovered != null) {
        ScreenCovered();
    }

    SDL_Delay(thirdTime);

    for (int i = 0; i < surface->h; i++) {
        lineRect.y = i;

        // Copy the line from oldSurface back to surface
        SDL_Rect dstRect = {0, i, surface->w, 1};
        SDL_BlitSurface(oldSurface, &lineRect, surface, &dstRect);
        SDL_UpdateWindowSurface(window);

        SDL_Delay(thirdTime / surface->h);
    }

    // Free the allocated memory
    SDL_FreeSurface(oldSurface);

    if (AnimComplete != null) {
        AnimComplete();
    }
}
