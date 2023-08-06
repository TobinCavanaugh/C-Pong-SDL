#include <stdio.h>
#include <stdbool.h>

#include "Collision.h"
#include "SDL.h"
#include "SharpKeywords.h"
#include "Ball.h"
#include "DeltaTime.h"
#include "ScreenTransitions.h"

int playerWidth = 20;
int playerHeight = 60;

int playerSpacing = 20;

float lPlayerY = 0;
float rPlayerY = 0;
float moveSensitivity = 400;

int leftPoints = 0;
int rightPoints = 0;

uint32_t lPlayerColor = 0x0000FF;
uint32_t rPlayerColor = 0xFF0000;

uint8_t *keyboardState;

bool pauseBalls = false;


//The surface that will be the screen
SDL_Surface *screenSurface = NULL;

//The window we'll be rendering to
SDL_Window *window = NULL;


SDL_Rect HandlePlayer(SDL_Scancode upButton, SDL_Scancode downButton, float *centerValue, int sideMultiplier) {
    if (keyboardState[downButton]) {
        *centerValue += (1.0f * GetDeltaTime()) * moveSensitivity;
    }

    if (keyboardState[upButton]) {
        *centerValue -= (1.0f * GetDeltaTime()) * moveSensitivity;
    }

    *centerValue = SDL_clamp(*centerValue, 0 - screenSurface->h / 2 + playerHeight / 2,
                             screenSurface->h / 2 - playerHeight + playerHeight / 2);


    //Get the side values
    int value = playerSpacing;

    if (sideMultiplier > 0) {
        value = screenSurface->w - playerWidth - playerSpacing;
    }

    //Create the player rect
    SDL_Rect playerRect = (SDL_Rect) {value,
                                      (int) *centerValue + screenSurface->h / 2.0f - playerHeight / 2,
                                      playerWidth,
                                      playerHeight};
    return playerRect;
}


void UpdateOneSide(SDL_Surface surface, int pointCount, int side, uint32_t color) {

    int height = 10;
    int width = 10;
    int spacing = 4;

    //Create the sample rect
    SDL_Rect sampleRect = (SDL_Rect) {0, 0, width, height};

    //Go through all the points
    for (int i = 0; i < pointCount; i++) {

        if (side < 0) {
            sampleRect.x = spacing;
        } else {
            sampleRect.x = screenSurface->w - width - spacing;
        }

        sampleRect.y = (i * height) + (i * spacing) + spacing;

        SDL_FillRect(&surface, &sampleRect, color, 0x00, 0x00);
    }
}

void UpdatePointsDisplay(SDL_Surface surface) {
    UpdateOneSide(surface, leftPoints, -1, lPlayerColor);
    UpdateOneSide(surface, rightPoints, 1, rPlayerColor);
}

void ResetPlayers() {
    lPlayerY = 0;
    rPlayerY = 0;
}

void OnSideHit(int side) {
    if (side > 0) {
        leftPoints++;
    }
    if (side < 0) {
        rightPoints++;
    }

    pauseBalls = true;

    var color = lPlayerColor;
    if (side < 0) {
        color = rPlayerColor;
    }


    ScreenWipe(window, 1000, color, ResetPlayers, null);


    pauseBalls = false;

    RestartDeltaTime();
}

int main(int argc, char *args[]) {

    //Initialize SDL and return if it fails
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 999;
    }

    //Create window
    window = SDL_CreateWindow("SDL Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640,
                              480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);


    //If window creation failed, return
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 777;
    }

    //Get window surface
    screenSurface = SDL_GetWindowSurface(window);
    playerHeight = screenSurface->clip_rect.h * .125f;
    playerWidth = screenSurface->clip_rect.w * 0.02125f;

    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;

    Ball ball = (Ball) {0x00FF00, screenSurface->w / 2, screenSurface->h / 2,
                        1, 0, .5f, 20};
    keyboardState = SDL_GetKeyboardState(NULL);

//    SDL_AddEventWatch(ResizeEvent, window);

    //Update Loop
    while (!quit) {
        // Poll for events
        while (SDL_PollEvent(&e)) {

            if (e.type == SDL_QUIT) {
                printf("\nQuitting...");
                quit = 1;
            }

            var windowEvent = e.window.event;
            //If the user is resizing the window or moving the window
            if (e.type == SDL_WINDOWEVENT &&
                (windowEvent == SDL_WINDOWEVENT_RESIZED || windowEvent == SDL_WINDOWEVENT_SIZE_CHANGED ||
                 windowEvent == SDL_WINDOWEVENT_MOVED)) {

                var oldRect = screenSurface->clip_rect;
                screenSurface = SDL_GetWindowSurface(window);

                //We recalculate the ball position here to keep it in the same relative position
                RecalculateBallPosition(&ball, oldRect, screenSurface->clip_rect);

                playerHeight = screenSurface->clip_rect.h * .125f;
                playerWidth = screenSurface->clip_rect.w * 0.02125f;

                //4:3 -> 400
                // 4 / 3 * 300

                moveSensitivity = (float) screenSurface->clip_rect.h / (float) screenSurface->clip_rect.w * 533.0f;

                //Restart the delta time so the ball doesnt jump through the wall
                RestartDeltaTime();
            }
        }

        UpdateDeltaTime();

        //Update the keyboardState
        keyboardState = SDL_GetKeyboardState(NULL);

        if (keyboardState[SDL_SCANCODE_P]) {
            ScreenWipe(window, 1000, rPlayerColor, null, null);
            RestartDeltaTime();
        }



        //Clear the screen
        SDL_FillRect(screenSurface, null, 0x00, 0x00, 0x00);

        //Update the left player
        var lPlayerRect = HandlePlayer(SDL_SCANCODE_W, SDL_SCANCODE_S, &lPlayerY, -1);
        SDL_FillRect(screenSurface, &lPlayerRect, lPlayerColor, 0x00, 0x00);

        //Update the right player
        var rPlayerRect = HandlePlayer(SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, &rPlayerY, 1);
        SDL_FillRect(screenSurface, &rPlayerRect, rPlayerColor, 0x00, 0x00);

        //Draw the dividing line
        int dividerWidth = screenSurface->w / 64;

        SDL_FillRect(screenSurface,
                     &(SDL_Rect) {screenSurface->w / 2 - dividerWidth / 2, 0, dividerWidth, screenSurface->h},
                     0x222244, 0x00, 0x00);


        //Update the ball's physics
        if (!pauseBalls) {
            DoBallUpdate(&ball, lPlayerRect, rPlayerRect, screenSurface->clip_rect, GetDeltaTime(), &OnSideHit);
        }

        RenderBall(*screenSurface, ball);

        UpdatePointsDisplay(*screenSurface);

        //Refresh the actual window
        SDL_UpdateWindowSurface(window);

    }

    return 0;
}
