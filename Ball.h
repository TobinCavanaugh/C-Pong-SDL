//
// Created by tobin on 8/5/2023.
//


#ifndef SDLATTEMPT2_BALL_H
#define SDLATTEMPT2_BALL_H

#include "Collision.h"
#include "SDL.h"
#include <stdint.h>
#include "SharpKeywords.h"

typedef struct {

    uint32_t color;

    float positionX;
    float positionY;

    float directionX;
    float directionY;

    float ballSpeed;

    int apothem;
} Ball;


SDL_Rect GetBallRect(const Ball ball);

void DoBallUpdate(Ball *ball,
                  const SDL_Rect lPlayer, const SDL_Rect rPlayer, const SDL_Rect screenRect,
                  float deltaTime, void (*SideFunction)(int));


void RenderBall(SDL_Surface sf, const Ball ball);

void ResetBall(Ball *b, SDL_Rect screen, int side);

void RecalculateBallPosition(Ball *b, SDL_Rect oldRect, SDL_Rect newRect);

#endif //SDLATTEMPT2_BALL_H
