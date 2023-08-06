//
// Created by tobin on 8/5/2023.
//

#include "DeltaTime.h"
#include "SDL.h"

uint32_t currentFrameTime = 0;
uint32_t prevFrameTime = 0;
float deltaTime;

float GetDeltaTime() {
    return deltaTime;
}

void RestartDeltaTime() {
    prevFrameTime = SDL_GetTicks();
    UpdateDeltaTime();
}

void UpdateDeltaTime() {
    //Update deltaTime
    currentFrameTime = SDL_GetTicks();
    deltaTime = ((float) currentFrameTime - (float) prevFrameTime) / 1000.0f;
    prevFrameTime = currentFrameTime;
}