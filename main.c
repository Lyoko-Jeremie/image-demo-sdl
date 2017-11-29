#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#define SDL_MAIN_HANDLED

#include "SDL.h"
#include "SDL_image.h"

#include "imgproc.h"
#include "highgui.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    // Initialize SDL
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return (1);

    if (initWindow("SDL Tutorial", 800, 600) != true)
        return (1);

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) {
        printf("IMG_Init: Failed to init required jpg and png support!\n");
        printf("IMG_Init: %s\n", IMG_GetError());
        // handle error
        return (1);
    }


    SDL_Surface *image;
//    image = IMG_Load("google-1132300-232400-B.png");
    image = IMG_Load("lena.png");
    if (!image) {
        printf("IMG_Load: %s\n", IMG_GetError());
        // handle error
        return (1);
    }

    printf("w:%d h:%d \n", image->w, image->h);
    printf("BytesPerPixel:%d \n", image->format->BytesPerPixel);


    if (SDL_MUSTLOCK(image) && SDL_LockSurface(image) != 0) {
        printf("SDL_LockSurface Failed.\n");
        // handle error
        return (1);
    }

    assert(image->w > 100 && image->h > 100);
    for (int y = 0; y != 100; ++y) {
        for (int x = 0; x != 100; ++x) {
            RGB rgb;
            getRGB(image, x, y, &rgb);

            rgb.r = 255;
            rgb.g = 255;
            rgb.b = 255;

            putRGB(image, x, y, &rgb);
        }
    }


    if (SDL_MUSTLOCK(image)) {
        SDL_UnlockSurface(image);
    }


    IMG_SavePNG(image, "t.png");


    showImageOnWindow(image);




    // Delay 2 seconds
    waitKey(20000);

    SDL_FreeSurface(image);
    image = NULL;

    closeWindow();

    // unload the dynamically loaded image libraries
    IMG_Quit();

    // Quit SDL
    SDL_Quit();


    return 0;
}

