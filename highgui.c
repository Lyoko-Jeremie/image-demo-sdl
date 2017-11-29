//
// Created by Jeremie on 2017/11/29.
//

#include "highgui.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE


//The window we'll be rendering to
FLAG_PRIVATE
SDL_Window *gWindow = NULL;

//The surface contained by the window
FLAG_PRIVATE
SDL_Surface *gScreenSurface = NULL;

FLAG_PUBLIC
bool initWindow(char *name, int w, int h) {
    //Initialization flag
    bool success = true;

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
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        success = false;
    } else {
        //Get window surface
        gScreenSurface = SDL_GetWindowSurface(gWindow);
    }

    return success;
}

FLAG_PUBLIC
void closeWindow() {
    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
}

FLAG_PUBLIC
void showImageOnWindow(SDL_Surface *image) {
    //Apply the image
    SDL_BlitSurface(image, NULL, gScreenSurface, NULL);
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
    for (int i = 0; i < time / timePiece; ++i) {
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




