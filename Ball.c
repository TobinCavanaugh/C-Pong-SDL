//
// Created by tobin on 8/5/2023.
//

#include "Ball.h"
#include <stdio.h>

int chosenSide = 1;

float ballEnglishMult = 1.0f;

SDL_Rect GetBallRect(const Ball ball) {
    SDL_Rect rect = (SDL_Rect) {ball.positionX - ball.apothem / 2, ball.positionY - ball.apothem / 2, ball.apothem,
                                ball.apothem};
    return rect;
}

float CalculateEnglish(float ballY, float playerY, float playerH) {
//    return 1;

    return ((playerY + (playerH / 2) - ballY) / playerH) * 2 * ballEnglishMult * -1;
}

//SDL_Surface *sff;
//
//void Set(SDL_Surface *sffff) {
//    sff = sffff;
//}

float GetBallAddition(Ball *ball, float directionAmount, float deltaTime, int screenDim) {
    float fractionX = directionAmount * deltaTime * ball->ballSpeed * (float) screenDim;
    return fractionX;
}

void DoBallUpdate(Ball *ball,
                  const SDL_Rect lPlayer, const SDL_Rect rPlayer, const SDL_Rect screenRect,
                  float deltaTime, void (*SideFunction)(int)) {

    ball->apothem = (screenRect.w + screenRect.h) / 56;

    //Update the ball's position based on the fraction of the screenRect's width and height
    ball->positionX += GetBallAddition(ball, ball->directionX, deltaTime, screenRect.w);
    ball->positionY += GetBallAddition(ball, ball->directionY, deltaTime, screenRect.h);

    var ballRect = GetBallRect(*ball);

    char collided = 0;

    //Left Player
    if (!collided && RectsOverlap(ballRect, lPlayer)) {
        ball->directionX *= -1;
        ball->positionX += 1;
        collided = 1;
//        ball->directionY = CalculateEnglish(ball->positionY, lPlayer.y, lPlayer.h);

        ball->directionY = CalculateEnglish(ball->positionY, lPlayer.y, lPlayer.h);
        collided = 1;
    }

    //Right player
    if (!collided && RectsOverlap(ballRect, rPlayer)) {
        ball->directionX *= -1;
        collided = 1;
        ball->positionX -= 1;
        ball->directionY = CalculateEnglish(ball->positionY, rPlayer.y, rPlayer.h);
        collided = 1;
    }

    const int thickness = 10;

    if (!collided) {
        SDL_Rect top = (SDL_Rect) {screenRect.x, -thickness, screenRect.w, thickness};

        // SDL_FillRect(sff, &top, 0xff00ff, 0x00, 0x00);

        if (RectsOverlap(ballRect, top)) {
            ball->directionY *= -1;
            ball->positionY += 1;
            collided = 1;
        }
    }

    if (!collided) {
        SDL_Rect bottom = (SDL_Rect) {screenRect.x, screenRect.h, screenRect.w, thickness};

        // SDL_FillRect(sff, &bottom, 0xff00ff, 0x00, 0x00);

        if (RectsOverlap(ballRect, bottom)) {
            ball->directionY *= -1;
            ball->positionY -= 1;
            collided = 1;
        }
    }

    if (!collided) {
        SDL_Rect leftRect = (SDL_Rect) {0 - thickness, screenRect.y, thickness, screenRect.h};

        // SDL_FillRect(sff, &leftRect, 0xff00ff, 0x00, 0x00);

        if (RectsOverlap(ballRect, leftRect)) {
            collided = 1;
            ResetBall(ball, screenRect, -1);
            SideFunction(-1);
        }
    }

    if (!collided) {
        SDL_Rect rightRect = (SDL_Rect) {screenRect.w, screenRect.y, thickness, screenRect.h};
        // SDL_FillRect(sff, &rightRect, 0xff00ff, 0x00, 0x00);

        if (RectsOverlap(ballRect, rightRect)) {
            collided = 1;
            ResetBall(ball, screenRect, 1);
            SideFunction(1);
        }
    }

    int bounds = 10;

    //If the ball gets out of bounds on the X axis
    if (ball->positionX < 0 - ball->apothem - bounds || ball->positionX + ball->apothem > screenRect.w + bounds) {
        ResetBall(ball, screenRect, chosenSide);
        chosenSide *= -1;
    }

    if (ball->positionY < 0 - ball->apothem - bounds || ball->positionY + ball->apothem > screenRect.h + bounds * 2) {
        ResetBall(ball, screenRect, chosenSide);
        chosenSide *= -1;
    }

//
//    //If the ball gets out of bounds on the Y axis
//    if (ball->positionY< (float) (-ball->apothem - bounds)
//                        || ball->positionY + (float) ball->apothem > (float)(screenRect.h + bounds)) {
//
//        ResetBall(ball, screenRect, 1);
//    }
}


void RecalculateBallPosition(Ball *b, SDL_Rect oldRect, SDL_Rect newRect) {
    b->positionX = (b->positionX) * ((float) newRect.w / (float) oldRect.w);
    b->positionY = (b->positionY) * ((float) newRect.h / (float) oldRect.h);
}

void ResetBall(Ball *b, SDL_Rect screen, int side) {
    b->positionX = (float) screen.w / 2;
    b->positionY = (float) screen.h / 2;
    b->directionX = (float) side;
    b->directionY = 0;
}

void RenderBall(SDL_Surface sf, const Ball ball) {
    var rect = GetBallRect(ball);
    SDL_FillRect(&sf, &rect, ball.color, 0x00, 0x00);
}
