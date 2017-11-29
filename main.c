#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "global_header.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    if (!initEverything()) {
        printf_s("init failed.");
        return (1);
    }

    if (initWindow("SDL Tutorial", 800, 600) != true) {
        printf_s("window create failed.");
        return (1);
    }

    {
        SDL_Surface *image;
        if (!loadImage("lena.png", &image)) {
            printf_s("image load failed.");
            return (1);
        }

        printf_s("w:%d h:%d \n", image->w, image->h);

        if (!lockSurface(image)) {
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

        unlockSurface(image);

        saveImage2PNG("t.png", image);
        drawImageToWindow(image);

        waitKey(0);
        deleteSurface(&image);
    }

    {
        SDL_Surface *picture;
        RGB rgbBG;
        rgbBG.r = 255;
        rgbBG.g = 0;
        rgbBG.b = 255;
        if (!createSurfaceWithRGB(500, 500, &rgbBG, &picture)) {
            return (1);
        }

        if (!lockSurface(picture)) {
            return (1);
        }

        assert(picture);
        assert(picture->w > 100 && picture->h > 100);
        for (int y = 0; y != 100; ++y) {
            for (int x = 0; x != 100; ++x) {
                RGB rgb;
                getRGB(picture, x, y, &rgb);

                rgb.r = 255;
                rgb.g = 255;
                rgb.b = 255;

                putRGB(picture, x, y, &rgb);
            }
        }

        unlockSurface(picture);

        clearWindowWithBlack();
        drawImageToWindow(picture);

        waitKey(0);
        deleteSurface(&picture);
    }

    closeEverything();

    return 0;
}

