//
// Created by Jeremie on 2017/11/29.
//

#ifndef DEMO_SDL_IMGPROC_H
#define DEMO_SDL_IMGPROC_H

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "SDL_image.h"

typedef struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB;

typedef struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
} RGBA;

struct SDL_Color;

void getPixel(SDL_Surface *surface, int x, int y, Uint32 *pixels);

void getRGB(SDL_Surface *surface, int x, int y, RGB *rgb);

void getRGBA(SDL_Surface *surface, int x, int y, RGBA *rgba);

void putPixel(SDL_Surface *surface, int x, int y, Uint32 *pixels);

void putRGB(SDL_Surface *surface, int x, int y, RGB *rgb);

void putRGBA(SDL_Surface *surface, int x, int y, RGBA *rgba);


#endif //DEMO_SDL_IMGPROC_H
