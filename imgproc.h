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

bool createSurfaceColorful(int w, int h, SDL_Surface **image);

bool createSurfaceWithRGB(int w, int h, RGB *rgb, SDL_Surface **image);

bool createSurfaceWithRGBA(int w, int h, RGBA *rgba, SDL_Surface **image);

bool createSurfaceGrayscale(int width, int height, Uint8 gray, SDL_Surface **image);

void cloneSurface(SDL_Surface *const *old, SDL_Surface **new);

void deleteSurface(SDL_Surface **image);

void clearSurfaceWithRGB(SDL_Surface *image, RGB *rgb);

void clearSurfaceWithRGBA(SDL_Surface *image, RGBA *rgba);

bool loadImage(const char *filePath, SDL_Surface **image);

bool saveImage2PNG(const char *filePath, SDL_Surface *surface);

bool saveImage2JPG(const char *filePath, SDL_Surface *surface);

bool lockSurface(SDL_Surface *image);

bool unlockSurface(SDL_Surface *image);

#endif //DEMO_SDL_IMGPROC_H
