//
// Created by Jeremie on 2017/11/29.
//
#include "imgproc.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE

FLAG_PRIVATE
Uint32 get_pixel32(SDL_Surface *surface, int x, int y) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *) surface->pixels;

    const Uint8 bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    // Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    //Get the requested pixel
    return pixels[(y * surface->pitch) + x * bpp];
    //    return pixels[(y * surface->w) + x];
}

FLAG_PRIVATE
void put_pixel32(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    //Convert the pixels to 32 bit
    Uint32 *pixels = (Uint32 *) surface->pixels;

    const Uint8 bpp = surface->format->BytesPerPixel;

    //Set the pixel
    pixels[(y * surface->pitch) + x * bpp] = pixel;
    //    pixels[(y * surface->w) + x] = pixel;
}

FLAG_PRIVATE
void rgb2pixel32(const SDL_Surface *surface, RGB *rgb, Uint32 *pixel) {
    *pixel = SDL_MapRGB(surface->format, rgb->r, rgb->g, rgb->b);
}

FLAG_PRIVATE
void pixel322rgb(const SDL_Surface *surface, const Uint32 *pixel, RGB *rgb) {
    SDL_GetRGB(*pixel, surface->format, &(rgb->r), &(rgb->g), &(rgb->b));
}

FLAG_PRIVATE
void rgba2pixel32(const SDL_Surface *surface, RGBA *rgba, Uint32 *pixel) {
    *pixel = SDL_MapRGBA(surface->format, rgba->r, rgba->g, rgba->b, rgba->a);
}

FLAG_PRIVATE
void pixel322rgba(const SDL_Surface *surface, const Uint32 *pixel, RGBA *rgba) {
    SDL_GetRGBA(*pixel, surface->format, &(rgba->r), &(rgba->g), &(rgba->b), &(rgba->a));
}

FLAG_PRIVATE
void PutPixel32(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    *((Uint32 *) pixel) = *color;
}

FLAG_PRIVATE
void GetPixel32(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint32));
    *color = *((Uint32 *) pixel);
}

FLAG_PRIVATE
void PutPixel24(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    pixel[0] = (Uint8) ((*color >> 24) & 0xFF);
    pixel[1] = (Uint8) ((*color >> 16) & 0xFF);
    pixel[2] = (Uint8) ((*color >> 8) & 0xFF);
#else
    pixel[0] = (Uint8) (*color & 0xFF);
    pixel[1] = (Uint8) ((*color >> 8) & 0xFF);
    pixel[2] = (Uint8) ((*color >> 16) & 0xFF);
#endif
}

FLAG_PRIVATE
void GetPixel24(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint8) * 3);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    //    pixel[0] = (Uint8) ((color >> 24) & 0xFF);
    //    pixel[1] = (Uint8) ((color >> 16) & 0xFF);
    //    pixel[2] = (Uint8) ((color >> 8) & 0xFF);

    (*color) = 0;
    Uint32 temp = 0;

    temp = 0;
    temp = pixel[0];
    (*color) |= temp;

    (*color) <<= 8;

    temp = 0;
    temp = pixel[1];
    (*color) |= temp;

    (*color) <<= 8;

    temp = 0;
    temp = pixel[2];
    (*color) |= temp;

    (*color) <<= 8;

#else
    //    pixel[0] = (Uint8) (color & 0xFF);
    //    pixel[1] = (Uint8) ((color >> 8) & 0xFF);
    //    pixel[2] = (Uint8) ((color >> 16) & 0xFF);

    (*color) = 0;
    Uint32 temp = 0;

    temp = 0;
    temp = pixel[2];
    (*color) |= temp;

    (*color) <<= 8;

    temp = 0;
    temp = pixel[1];
    (*color) |= temp;

    (*color) <<= 8;

    temp = 0;
    temp = pixel[0];
    (*color) |= temp;


#endif
}

FLAG_PRIVATE
void PutPixel16(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint16));
    *((Uint16 *) pixel) = (Uint16) (*color & 0xFFFF);
}

FLAG_PRIVATE
void GetPixel16(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint16));
    //    *((Uint16 *) pixel) = (Uint16) (color & 0xFFFF);
    *color = (Uint32) (*((Uint16 *) pixel) & 0xFFFF);
}

FLAG_PRIVATE
void PutPixel8(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint8));
    *pixel = (Uint8) (*color & 0xFF);
}

FLAG_PRIVATE
void GetPixel8(SDL_Surface *surface, int x, int y, Uint32 *color) {
    Uint8 *pixel = (Uint8 *) surface->pixels;
    pixel += (y * surface->pitch) + (x * sizeof(Uint8));
    //    *pixel = (Uint8) (color & 0xFF);
    *color = (Uint32) (*((Uint8 *) pixel) & 0xFF);
}


FLAG_PUBLIC
void getPixel(SDL_Surface *surface, int x, int y, Uint32 *pixels) {
    const Uint8 bpp = surface->format->BytesPerPixel;
    switch (bpp) {
        case 4:
            GetPixel32(surface, x, y, pixels);
            break;
        case 3:
            GetPixel24(surface, x, y, pixels);
            break;
        case 2:
            GetPixel16(surface, x, y, pixels);
            break;
        case 1:
            GetPixel8(surface, x, y, pixels);
            break;
        default:
            assert(0);
            break;
    }
}

FLAG_PUBLIC
void getRGB(SDL_Surface *surface, int x, int y, RGB *rgb) {
    Uint32 pixels;
    getPixel(surface, x, y, &pixels);
    pixel322rgb(surface, &pixels, rgb);
}

FLAG_PUBLIC
void getRGBA(SDL_Surface *surface, int x, int y, RGBA *rgba) {
    Uint32 pixels;
    getPixel(surface, x, y, &pixels);
    pixel322rgba(surface, &pixels, rgba);
}

FLAG_PUBLIC
void putPixel(SDL_Surface *surface, int x, int y, Uint32 *pixels) {
    const Uint8 bpp = surface->format->BytesPerPixel;
    switch (bpp) {
        case 4:
            PutPixel32(surface, x, y, pixels);
            break;
        case 3:
            PutPixel24(surface, x, y, pixels);
            break;
        case 2:
            PutPixel16(surface, x, y, pixels);
            break;
        case 1:
            PutPixel8(surface, x, y, pixels);
            break;
        default:
            assert(0);
            break;
    }
}

FLAG_PUBLIC
void putRGB(SDL_Surface *surface, int x, int y, RGB *rgb) {
    Uint32 pixels;
    rgb2pixel32(surface, rgb, &pixels);
    putPixel(surface, x, y, &pixels);
}

FLAG_PUBLIC
void putRGBA(SDL_Surface *surface, int x, int y, RGBA *rgba) {
    Uint32 pixels;
    rgba2pixel32(surface, rgba, &pixels);
    putPixel(surface, x, y, &pixels);
}


FLAG_PUBLIC
bool createSurfaceColorful(int width, int height, SDL_Surface **image) {
    RGB rgb;
    rgb.r = 0;
    rgb.g = 0;
    rgb.b = 0;
    createSurfaceWithRGB(width, height, &rgb, image);
}

FLAG_PUBLIC
bool createSurfaceWithRGB(int width, int height, RGB *rgb, SDL_Surface **image) {
    RGBA rgba;
    rgba.r = rgb->r;
    rgba.g = rgb->g;
    rgba.b = rgb->b;
    rgba.a = 0;
    createSurfaceWithRGBA(width, height, &rgba, image);
}

FLAG_PUBLIC
bool createSurfaceWithRGBA(int width, int height, RGBA *rgba, SDL_Surface **image) {

    //    Uint32 rmask, gmask, bmask, amask;
    //    // SDL interprets each pixel as a 32-bit number, so our masks must depend on the endianness (byte order) of the machine
    //#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    //    rmask = 0xff000000;
    //    gmask = 0x00ff0000;
    //    bmask = 0x0000ff00;
    //    amask = 0x000000ff;
    //#else
    //    rmask = 0x000000ff;
    //    gmask = 0x0000ff00;
    //    bmask = 0x00ff0000;
    //    amask = 0xff000000;
    //#endif
    //    *image = SDL_CreateRGBSurface(0, width, height, 32, rmask, gmask, bmask, amask);

    *image = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

    if (!*image) {
        printf_s("Surface create failed.");
        printf_s("Error :%s", SDL_GetError());
        return false;
    }

    clearSurfaceWithRGBA(*image, rgba);

    return true;
}

FLAG_PUBLIC
bool createSurfaceGrayscale(int width, int height, Uint8 gray, SDL_Surface **image) {

    *image = SDL_CreateRGBSurface(0, width, height, 8, 0, 0, 0, 0);

    if (!*image) {
        printf_s("Surface create failed.");
        printf_s("Error :%s", SDL_GetError());
        return false;
    }

    SDL_Color colors[256];
    for (int i = 0; i < 256; i++) {
        colors[i].r = colors[i].g = colors[i].b = (Uint8) i;
    }
    SDL_SetPaletteColors((*image)->format->palette, colors, 0, 256);

    SDL_FillRect(*image, NULL, gray);

    return true;
}

FLAG_PUBLIC
void deleteSurface(SDL_Surface **image) {
    if (*image) {
        SDL_FreeSurface(*image);
        *image = NULL;
    }
}

FLAG_PUBLIC
void clearSurfaceWithRGB(SDL_Surface *image, RGB *rgb) {
    Uint32 color;
    rgb2pixel32(image, rgb, &color);
    SDL_FillRect(image, NULL, color);
}

FLAG_PUBLIC
void clearSurfaceWithRGBA(SDL_Surface *image, RGBA *rgba) {
    Uint32 color;
    rgba2pixel32(image, rgba, &color);
    SDL_FillRect(image, NULL, color);
}

FLAG_PUBLIC
bool loadImage(const char *filePath, SDL_Surface **image) {
    *image = IMG_Load(filePath);
    if (!*image) {
        printf_s("IMG_Load: %s\n", IMG_GetError());
        return false;
    }
    return true;
}

FLAG_PUBLIC
bool saveImage2PNG(const char *filePath, SDL_Surface *surface) {
    IMG_SavePNG(surface, filePath);
    return true;
}

FLAG_PUBLIC
bool saveImage2JPG(const char *filePath, SDL_Surface *surface) {
    IMG_SaveJPG(surface, filePath, 100);
    return true;
}

FLAG_PUBLIC
bool lockSurface(SDL_Surface *image) {
    if (SDL_MUSTLOCK(image) && SDL_LockSurface(image) != 0) {
        printf_s("SDL_LockSurface Failed.\n");
        return false;
    }
    return true;
}

FLAG_PUBLIC
bool unlockSurface(SDL_Surface *image) {
    if (SDL_MUSTLOCK(image)) {
        SDL_UnlockSurface(image);
    }
    return true;
}

FLAG_PUBLIC
void cloneSurface(SDL_Surface *const *old, SDL_Surface **new) {
    if (*new) {
        SDL_FreeSurface(*new);
        *new = NULL;
    }
    SDL_Surface *const ptr = (*old);
    *new = SDL_ConvertSurface(ptr, ptr->format, 0);
}

