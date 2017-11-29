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


//The window we'll be rendering to
FLAG_PRIVATE
SDL_Window *gWindow = NULL;

//The surface contained by the window
FLAG_PRIVATE
SDL_Surface *gWindowScreenSurface = NULL;

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
        printf_s("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    //Get window surface
    gWindowScreenSurface = SDL_GetWindowSurface(gWindow);

    //    // We must call SDL_CreateRenderer in order for draw calls to affect this window.
    //    gWindowRenderer = SDL_CreateRenderer(gWindow, -1, 0);
    //    // Select the color for drawing. It is set to red here.
    //    SDL_SetRenderDrawColor(gWindowRenderer, 255, 0, 0, 255);
    //    // Clear the entire screen to our selected color.
    //    SDL_RenderClear(gWindowRenderer);
    //    // Up until now everything was drawn behind the scenes.
    //    // This will show the new, red contents of the window.
    //    SDL_RenderPresent(gWindowRenderer);

    return true;
}

FLAG_PUBLIC
void closeWindow() {
    if (gWindow) {
        //Destroy window
        SDL_DestroyWindow(gWindow);
        gWindow = NULL;
    }
}

FLAG_PUBLIC
void drawImageToWindow(SDL_Surface *image) {
    if (!gWindowScreenSurface) {
        return;
    }
    //Apply the image
    SDL_BlitSurface(image, NULL, gWindowScreenSurface, NULL);
    //Update the surface
    SDL_UpdateWindowSurface(gWindow);
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
        //Handle events on queue
        if (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                // quit
                return SDL_SCANCODE_UNKNOWN;
            }
            switch (e.type) {
                case SDL_QUIT:
                    // TODO quit
                    break;
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
    if (!gWindowScreenSurface) {
        return;
    }
    Uint32 color;
    rgba2pixel32(gWindowScreenSurface, rgba, &color);
    SDL_FillRect(gWindowScreenSurface, NULL, color);
}

FLAG_PUBLIC
void clearWindowWithRGB(RGB *rgb) {
    if (!gWindowScreenSurface) {
        return;
    }
    Uint32 color;
    rgb2pixel32(gWindowScreenSurface, rgb, &color);
    SDL_FillRect(gWindowScreenSurface, NULL, color);
}


