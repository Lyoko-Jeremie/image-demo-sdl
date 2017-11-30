//
// Created by Jeremie on 2017/11/29.
//

#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "highgui.h"
#include "imgproc.h"

#define FLAG_PUBLIC
#define FLAG_PROTECTED
#define FLAG_PRIVATE


#define WINDOW_DRAW_MODE_TEXTURE 1
#define WINDOW_DRAW_MODE_SURFACE 2

// use which mode to draw window
#define WINDOW_DRAW_MODE WINDOW_DRAW_MODE_TEXTURE
//#define WINDOW_DRAW_MODE WINDOW_DRAW_MODE_SURFACE

//The window we'll be rendering to
FLAG_PRIVATE
SDL_Window *gWindow = NULL;

//The surface contained by the window
FLAG_PRIVATE
SDL_Surface *gWindowScreenSurface = NULL;

FLAG_PRIVATE
SDL_Renderer *gWindowRenderer = NULL;

FLAG_PRIVATE
SDL_Texture *gTextureLast = NULL;

FLAG_PRIVATE
SDL_Surface *gSurfaceLast = NULL;

FLAG_PRIVATE
double gScaleLast = 1.0;

FLAG_PUBLIC
bool initWindow(char *name, int w, int h) {
    if (gWindow) {
        return false;
    }
    //Create window
    gWindow = SDL_CreateWindow(
            name,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            w,
            h,
            SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (gWindow == NULL) {
        fprintf_s(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    //Get window surface
    gWindowScreenSurface = SDL_GetWindowSurface(gWindow);

#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_TEXTURE
    // We must call SDL_CreateRenderer in order for draw calls to affect this window.
    gWindowRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    // Select the color for drawing. It is set to black here.
    SDL_SetRenderDrawColor(gWindowRenderer, 0, 0, 0, 255);
    // Clear the entire screen to our selected color.
    SDL_RenderClear(gWindowRenderer);
    // Up until now everything was drawn behind the scenes.
    // This will show the new, black contents of the window.
    SDL_RenderPresent(gWindowRenderer);
#endif  // WINDOW_DRAW_MODE_TEXTURE

    return true;
}

FLAG_PRIVATE
void updateRender() {
    if (gTextureLast) {
        int dw = 0, dh = 0;
        int sw = 0, sh = 0;
        SDL_GetRendererOutputSize(gWindowRenderer, &dw, &dh);
        SDL_QueryTexture(gTextureLast, NULL, NULL, &sw, &sh);
        if (dw > 0 && dh > 0 && sw > 0 && sh > 0) {
            sw *= gScaleLast;
            sh *= gScaleLast;

            SDL_Rect r, t;

            r.x = r.y = t.x = t.y = 0;
            r.w = t.w = dw < sw ? dw : sw;
            r.h = t.h = dh < sh ? dh : sh;

            t.w /= gScaleLast;
            t.h /= gScaleLast;

            SDL_RenderCopy(gWindowRenderer, gTextureLast, &t, &r);
            SDL_RenderPresent(gWindowRenderer);
        }
    }
}

FLAG_PRIVATE
void updateSurface() {
    if (gSurfaceLast) {
        //Apply the image
        SDL_BlitSurface(gSurfaceLast, NULL, gWindowScreenSurface, NULL);
        //Update the surface
        SDL_UpdateWindowSurface(gWindow);
    }
}

FLAG_PRIVATE
void destroyTextureLast() {
    if (gTextureLast) {
        SDL_DestroyTexture(gTextureLast);
        gTextureLast = NULL;
    }
}

FLAG_PRIVATE
void destroySurfaceLast() {
    if (gSurfaceLast) {
        SDL_FreeSurface(gSurfaceLast);
        gSurfaceLast = NULL;
    }
}

FLAG_PRIVATE
void drawImageToWindowSurface(SDL_Surface *image, double scale) {
    fprintf_s(stderr, "Warning : scale on WINDOW_DRAW_MODE_SURFACE are not implement.\n");
    if (!gWindowScreenSurface) {
        return;
    }
    cloneSurface(&image, &gSurfaceLast);
    updateSurface();
}

FLAG_PRIVATE
void drawImageToWindowTexture(SDL_Surface *image, double scale) {
    if (!gWindowRenderer) {
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(gWindowRenderer, image);

    if (texture == NULL) {
        fprintf_s(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        return;
    }

    destroyTextureLast();

    gTextureLast = texture;
    gScaleLast = scale;
    updateRender();
}

FLAG_PUBLIC
void drawImageToWindow(SDL_Surface *image) {
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_TEXTURE
    drawImageToWindowTexture(image, 1);
#endif  // WINDOW_DRAW_MODE_TEXTURE
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_SURFACE
    drawImageToWindowSurface(image, 1);
#endif  // WINDOW_DRAW_MODE_SURFACE
}

FLAG_PUBLIC
void drawImageToWindowWithScale(SDL_Surface *image, double scale) {
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_TEXTURE
    drawImageToWindowTexture(image, scale);
#endif  // WINDOW_DRAW_MODE_TEXTURE
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_SURFACE
    drawImageToWindowSurface(image, scale);
#endif  // WINDOW_DRAW_MODE_SURFACE
}

/**
 *
 * @param time ms
 * @return SDL_Scancode | 0 when no key press
 */
FLAG_PUBLIC
SDL_Scancode waitKey(Uint32 time) {
    const int timePiece = 5;
    //Event handler
    SDL_Event e;
    int loop = time != 0 ? (time >= timePiece ? time / timePiece : 1) : INT_MAX;
    for (int i = 0; i < loop; ++i) {
        SDL_Delay(timePiece);
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_TEXTURE
        updateRender();
#endif  // WINDOW_DRAW_MODE_TEXTURE
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_SURFACE
        updateSurface();
#endif  // WINDOW_DRAW_MODE_SURFACE
        //Handle events on queue
        if (SDL_PollEvent(&e) != 0) {
            switch (e.type) {
                case SDL_QUIT:
                    // quit
                    return e.key.keysym.scancode;
                case SDL_KEYUP:
                    return e.key.keysym.scancode;
                default:
                    break;
            }
        }
    }

    return SDL_SCANCODE_UNKNOWN;
}

FLAG_PROTECTED
extern void rgb2pixel32(const SDL_Surface *surface, RGB *rgb, Uint32 *pixel);

FLAG_PROTECTED
extern void rgba2pixel32(const SDL_Surface *surface, RGBA *rgba, Uint32 *pixel);

FLAG_PUBLIC
void clearWindowWithBlack() {
    RGB rgb;
    rgb.r = 0;
    rgb.g = 0;
    rgb.b = 0;
    clearWindowWithRGB(&rgb);
}

FLAG_PUBLIC
void clearWindowWithRGBA(RGBA *rgba) {
    if (!gWindowRenderer) {
        return;
    }
    gScaleLast = 0;
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_TEXTURE
    destroyTextureLast();
    SDL_SetRenderDrawColor(gWindowRenderer, rgba->r, rgba->g, rgba->b, rgba->a);
    SDL_RenderClear(gWindowRenderer);
    SDL_RenderPresent(gWindowRenderer);
#endif  // WINDOW_DRAW_MODE_TEXTURE
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_SURFACE
    Uint32 color;
    rgba2pixel32(gWindowScreenSurface, rgba, &color);
    SDL_FillRect(gWindowScreenSurface, NULL, color);
#endif  // WINDOW_DRAW_MODE_SURFACE
}

FLAG_PUBLIC
void clearWindowWithRGB(RGB *rgb) {
    if (!gWindowRenderer) {
        return;
    }
    gScaleLast = 0;
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_TEXTURE
    destroyTextureLast();
    SDL_SetRenderDrawColor(gWindowRenderer, rgb->r, rgb->g, rgb->b, 255);
    SDL_RenderClear(gWindowRenderer);
    SDL_RenderPresent(gWindowRenderer);
#endif  // WINDOW_DRAW_MODE_TEXTURE
#if WINDOW_DRAW_MODE == WINDOW_DRAW_MODE_SURFACE
    Uint32 color;
    rgb2pixel32(gWindowScreenSurface, rgb, &color);
    SDL_FillRect(gWindowScreenSurface, NULL, color);
#endif  // WINDOW_DRAW_MODE_SURFACE
}

FLAG_PUBLIC
void closeWindow() {
    destroySurfaceLast();
    destroyTextureLast();
    if (gWindowRenderer) {
        SDL_DestroyRenderer(gWindowRenderer);
        gWindowRenderer = NULL;
    }
    if (gWindow) {
        //Destroy window
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
    }
}

