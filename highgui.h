//
// Created by Jeremie on 2017/11/29.
//

#ifndef DEMO_SDL_HIGHGUI_H
#define DEMO_SDL_HIGHGUI_H

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "SDL.h"

bool initWindow(char *name, int w, int h);

void closeWindow();

void showImageOnWindow(SDL_Surface *image);

SDL_Scancode waitKey(Uint32 time);


#endif //DEMO_SDL_HIGHGUI_H
