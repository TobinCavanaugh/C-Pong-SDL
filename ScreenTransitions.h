//
// Created by tobin on 8/5/2023.
//

#ifndef SDLATTEMPT2_SCREENTRANSITIONS_H
#define SDLATTEMPT2_SCREENTRANSITIONS_H

#include "SDL_surface.h"
#include "SDL.h"

void ScreenWipe(SDL_Window *window, int totalTimeMs, uint32_t color, void (*ScreenCovered)(), void (*AnimComplete)());

#endif //SDLATTEMPT2_SCREENTRANSITIONS_H
