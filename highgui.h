//
// Created by Jeremie on 2017/11/29.
//

#ifndef DEMO_SDL_HIGHGUI_H
#define DEMO_SDL_HIGHGUI_H

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "SDL.h"
#include "imgproc.h"

bool initWindow(char *name, int w, int h);

void closeWindow();

void drawImageToWindow(SDL_Surface *image);

void drawImageToWindowWithScale(SDL_Surface *image, double scale);

SDL_Scancode waitKey(Uint32 time);

void clearWindowWithRGB(RGB *rgb);

void clearWindowWithRGBA(RGBA *rgba);

void clearWindowWithBlack();

#endif //DEMO_SDL_HIGHGUI_H
