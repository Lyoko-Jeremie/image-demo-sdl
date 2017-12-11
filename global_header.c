//
// Created by Jeremie on 2017/11/29.
//
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "global_header.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE

FLAG_PRIVATE
bool isInited = false;

FLAG_PUBLIC
bool initEverything() {
    if (isInited) {
        return isInited;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF | IMG_INIT_WEBP;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) {
        fprintf(stderr,"IMG_Init: Failed to init required jpg and png support!\n");
        fprintf(stderr,"IMG_Init: %s\n", IMG_GetError());
        // handle error
        return false;
    }

    isInited = true;
    return true;
}

FLAG_PUBLIC
bool closeEverything() {
    closeWindow();
    // unload the dynamically loaded image libraries
    IMG_Quit();
    // Quit SDL
    SDL_Quit();
}


