//
// Created by tobin on 8/5/2023.
//

#include "Collision.h"

char RectsOverlap(const SDL_Rect a, const SDL_Rect b) {
    if (a.x + a.w < b.x || a.x > b.x + b.w) {
        return 0;
    }

    if (a.y + a.h < b.y || a.y > b.y + b.h) {
        return 0;
    }

    return 1;
}