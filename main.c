#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#include "global_header.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE

bool erode(SDL_Surface **imageGrayPtr);

bool dilate(SDL_Surface **imageGrayPtr);

bool add(SDL_Surface **imageAPtr, SDL_Surface **imageBPtr, SDL_Surface **imageCPtr);

bool sub(SDL_Surface **imageAPtr, SDL_Surface **imageBPtr, SDL_Surface **imageCPtr);

bool negative(SDL_Surface **imagePtr);

bool addNumber(SDL_Surface **imagePtr, int num);

bool subNumber(SDL_Surface **imagePtr, int num);

bool divNumber(SDL_Surface **imagePtr, int num);

bool mulNumber(SDL_Surface **imagePtr, int num);

bool medianBlur(SDL_Surface **imageGrayPtr);

bool medianBlurN(SDL_Surface **imageGrayPtr, int xN, int yN);


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    // 初始化图像环境
    if (!initEverything()) {
        printf("init failed.");
        return (1);
    }

    // 初始化显示窗口  并设置初始窗口大小为 宽800像素 高600像素
    if (initWindow("SDL Tutorial", 800, 600) != true) {
        printf("window create failed.");
        return (1);
    }

    // 从图片文件读取图像，在图像左上角画一个矩形，显示并保存图像
    {
        // 创建一个用来持有图像的图像表面指针
        SDL_Surface *image;
        // 从图片文件读取图像表面并由image持有
        if (!loadImage("lena.png", &image)) {
            printf("image load failed.");
            return (1);
        }

        // 打出显示图像表面的宽和高
        printf("w:%d h:%d \n", image->w, image->h);

        // 锁定图像表面
        // 注意：在操作图像表面的像素色彩是必须要锁定图像表面
        if (!lockSurface(image)) {
            return (1);
        }

        // 检查图像的宽和高是否都大于100
        assert((image->w > 100) && (image->h > 100));
        // 下面的两个for循环会遍历图像左上角的100x100范围内的所有像素
        // 小心：如果访问(读取或者修改)的像素位置超过了图像本身的大小，会导致程序崩溃
        // 小提示：计算机图像的坐标原点(0,0)通常在左上角；并且以右方向为x轴正方向，下方向为y轴正方向
        for (int y = 0; y != 100; ++y) {
            for (int x = 0; x != 100; ++x) {

                // 读取特定位置RGB色彩的方法
                {
                    RGB rgb;
                    // 从图像表面的特定位置读取一个RGB像素
                    getRGB(image, x, y, &rgb);
                }

                // 修改特定位置RGB色彩的方法
                {
                    RGB rgb;

                    // 将名为rgb的RGB像素的色彩设置为白色(255,255,255)
                    rgb.r = 255;
                    rgb.g = 255;
                    rgb.b = 255;

                    // 将RGB像素写入图像表面的特定位置
                    putRGB(image, x, y, &rgb);
                }

            }
        }

        // 解锁图像表面
        // 注意：在操作完毕图像表面的像素色彩后必须要解锁图像表面才能做其他的操作
        unlockSurface(image);

        // 把图像表面保存为图片文件
        saveImage2PNG("t.png", image);
        // 把图像表面image持有的图像绘制到窗口上
        drawImageToWindow(image);

        // 让显示窗口可以响应键盘输入
        waitKey(0);

        // 销毁image持有的图像表面
        deleteSurface(&image);
    }

    // 创建一个空白的图像，在图像左上角画一个矩形，销毁图像
    {
        // 创建一个用来持有图像的图像表面指针
        SDL_Surface *picture;

        // 创建一个宽800高600的新的黑色的彩色图像表面并由picture持有
        if (!createSurfaceColorful(800, 600, &picture)) {
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

                rgb.r = 255;
                rgb.g = 255;
                rgb.b = 255;

                putRGB(picture, x, y, &rgb);
            }
        }

        unlockSurface(picture);

        // 使用黑色清空窗口
        clearWindowWithBlack();
        // 把图像表面picture持有的图像绘制到窗口上
        drawImageToWindow(picture);

        waitKey(0);
        // 销毁picture持有的图像表面
        deleteSurface(&picture);
    }

    // 下面这段代码会在宽高均为500的白色的图像左上角画一个绿色的空心矩形，并绘制一个蓝色的实心圆，最后显示并保存图像
    // 矩形的坐标信息(x:50,y:50,w:101,h:101)
    // 圆形的坐标信息(x:150,y:150,r:50)
    {
        SDL_Surface *picture;

        if (!createSurfaceColorful(500, 500, &picture)) {
            return (1);
        }

        // 将picture持有的图像表面清空为白色
        {
            RGB rgb;
            rgb.r = 255;
            rgb.g = 255;
            rgb.b = 255;
            clearSurfaceWithRGB(picture, &rgb);
        }

        if (!lockSurface(picture)) {
            return (1);
        }
        assert(picture);
        assert(picture->w > 300 && picture->h > 300);
        for (int y = 50; y != 250; ++y) {
            for (int x = 50; x != 250; ++x) {
                // 使用勾股定理求像素坐标到点(100,100)的距离，绘制一个半径25的蓝色圆形
                if ((pow(150 - x, 2) + pow(150 - y, 2)) < pow(50, 2)) {
                    RGB rgb;
                    rgb.r = 0;
                    rgb.g = 0;
                    rgb.b = 255;
                    putRGB(picture, x, y, &rgb);
                }
            }
        }
        for (int y = 50; y != 151; ++y) {
            for (int x = 50; x != 151; ++x) {
                // 只将矩形边框处的颜色设置为绿色
                if (y == 50 || y == 150 || x == 50 || x == 150) {
                    RGB rgb;
                    rgb.r = 0;
                    rgb.g = 255;
                    rgb.b = 0;
                    putRGB(picture, x, y, &rgb);
                }
            }
        }
        unlockSurface(picture);

        clearWindowWithBlack();
        drawImageToWindow(picture);
        saveImage2PNG("p.png", picture);

        waitKey(0);
        deleteSurface(&picture);
    }

    // 下面这段代码会创建一个简单的灰度图像，并使用渐变填充这个图像，最后显示并保存
    {
        SDL_Surface *picture;

        // 创建一个宽800高600的新的初始值为0(黑色)的灰度图像表面并由picture持有
        if (!createSurfaceGrayscale(256, 256, 0, &picture)) {
            return (1);
        }

        if (!lockSurface(picture)) {
            return (1);
        }

        assert(picture);
        assert(picture->w >= 256 && picture->h >= 256);
        // 从图像表面中获取图像的像素起点
        Uint8 *basePtr = (Uint8 *) picture->pixels;
        for (int y = 0; y != 256; ++y) {
            for (int x = 0; x != 256; ++x) {
                // 设置当前下标所在处的像素     注意：像素的值总是介于0~255之间
                basePtr[y * picture->pitch + x] = (Uint8) ((x + y) / 2);
                // 小提示：计算机在内存中存储图像时，为了提高性能，每一行的长度可能不是图像的真实长度，
                //      计算机可能会在每一行的末尾添加几个无用的字节以便将图像的每一行对齐到某一个整数，
                //      并且在不同的计算机上，这个对齐的数字也有可能不尽相同。
                //   故，在使用图像坐标计算真实的图像内存地址时，
                //      行下标y需要乘以图像在内存中存储的每行填充后的宽度(pitch)才能计算出当前行在内存中的真实的起始地址。
                // 注意：只有单通道的灰度图像可以使用此方式进行操作，
                //      彩色图像的存储方式与单通道图像有所不同，请使用putRGB和getRGB来操作彩色图像
                //      当然，也可以使用PutPixel来操作单通道的灰度图像

            }
        }

        unlockSurface(picture);

        clearWindowWithBlack();
        drawImageToWindowWithScale(picture, 10);
        saveImage2BMP("g.bmp", picture);

        waitKey(0);
        deleteSurface(&picture);
    }

    // 下面这段代码会从文件中读取图像，并转换为单通道灰度图像，然后对灰度图像执行操作，并显示操作后的图像
    {
        // 创建一个用来持有图像的图像表面指针
        SDL_Surface *image;
        // 从图片文件读取图像表面并由image持有
        if (!loadImage("lena.png", &image)) {
            printf("image load failed.");
            return (1);
        }

        // 打出显示图像表面的宽和高
        printf("w:%d h:%d \n", image->w, image->h);

        // 创建一个用来持有单通道灰度图像的图像表面指针
        SDL_Surface *imageGray;
        // 转换RGB彩色图像到单通道灰度图像
        if (!cvtColorFromRGB2Gray(image, &imageGray)) {
            printf("image covert failed.");
            return (1);
        }

        if (!lockSurface(imageGray)) {
            return (1);
        }

        assert(imageGray);
        // 从图像表面中获取图像的像素起点
        Uint8 *basePtr = (Uint8 *) imageGray->pixels;
        for (int i = 0; i != 10000; ++i) {
            basePtr[rand() % imageGray->h * imageGray->pitch + rand() % imageGray->w]
                    = (Uint8) (rand() % 2 == 0 ? 0 : 255);
        }

        unlockSurface(imageGray);
        printf("rand\n");

        clearWindowWithBlack();
        drawImageToWindowWithScale(imageGray, 1);
        waitKey(0);

//        medianBlurN(&imageGray, 15, 15);
//        printf("medianBlurN\n");
        medianBlur(&imageGray);
        printf("medianBlur\n");

        clearWindowWithBlack();
        drawImageToWindowWithScale(imageGray, 1);
        waitKey(0);

//        if (!lockSurface(imageGray)) {
//            return (1);
//        }
//
//        assert(imageGray);
//        // 从图像表面中获取图像的像素起点
//        Uint8 *basePtr = (Uint8 *) imageGray->pixels;
//        for (int y = 0; y != imageGray->h; ++y) {
//            for (int x = 0; x != imageGray->w; ++x) {
//                // 从单通道灰度图像中读取一个像素
//                Uint8 pixel = basePtr[y * imageGray->pitch + x];
//                if (pixel < 128) {
//                    pixel = 0;
//                } else {
//                    pixel = 255;
//                }
//                // 将一个像素写入到单通道图像
//                basePtr[y * imageGray->pitch + x] = pixel;
//            }
//        }
//
//        unlockSurface(imageGray);

//        if (true) {
//            for (int i = 0; i != 5; ++i) {
//                dilate(&imageGray);
//            }
//        }
//
//        if (true) {
//            for (int i = 0; i != 5; ++i) {
//                erode(&imageGray);
//            }
//        }
//
//        negative(&imageGray);

        clearWindowWithBlack();
//        saveImage2BMP("g.bmp", imageGray);

        drawImageToWindowWithScale(imageGray, 1);
        waitKey(0);

        deleteSurface(&imageGray);
        deleteSurface(&image);
    }

    // 关闭图像环境
    closeEverything();

    printf("end.");
    return 0;
}


bool erode(SDL_Surface **imageGrayPtr) {
    SDL_Surface *imageGray = *imageGrayPtr;
    SDL_Surface *imageGray2 = NULL;
    cloneSurface(imageGrayPtr, &imageGray2);
    if (!lockSurface(imageGray2)) {
        deleteSurface(&imageGray2);
        return false;
    }
    assert(imageGray2);
    Uint8 *basePtr = (Uint8 *) imageGray->pixels;
    Uint8 *basePtr2 = (Uint8 *) imageGray2->pixels;
    for (int y = 1; y != imageGray2->h - 1; ++y) {
        for (int x = 1; x != imageGray2->w - 1; ++x) {
            if (basePtr[y * imageGray->pitch + x] == 255) {
                if (
                        basePtr[(y - 1) * imageGray->pitch + x - 1] == 0 ||
                        basePtr[(y - 1) * imageGray->pitch + x] == 0 ||
                        basePtr[(y - 1) * imageGray->pitch + x + 1] == 0 ||

                        basePtr[(y) * imageGray->pitch + x - 1] == 0 ||
                        basePtr[(y) * imageGray->pitch + x] == 0 ||
                        basePtr[(y) * imageGray->pitch + x + 1] == 0 ||

                        basePtr[(y + 1) * imageGray->pitch + x - 1] == 0 ||
                        basePtr[(y + 1) * imageGray->pitch + x] == 0 ||
                        basePtr[(y + 1) * imageGray->pitch + x + 1] == 0
                        ) {
                    basePtr2[y * imageGray2->pitch + x] = 0;
                }
            }
        }
    }
    unlockSurface(imageGray2);
    cloneSurface(&imageGray2, imageGrayPtr);
    deleteSurface(&imageGray2);
    return true;
}

bool dilate(SDL_Surface **imageGrayPtr) {
    SDL_Surface *imageGray = *imageGrayPtr;
    SDL_Surface *imageGray2 = NULL;
    cloneSurface(imageGrayPtr, &imageGray2);
    if (!lockSurface(imageGray2)) {
        deleteSurface(&imageGray2);
        return false;
    }
    assert(imageGray2);
    Uint8 *basePtr = (Uint8 *) imageGray->pixels;
    Uint8 *basePtr2 = (Uint8 *) imageGray2->pixels;
    for (int y = 1; y != imageGray2->h - 1; ++y) {
        for (int x = 1; x != imageGray2->w - 1; ++x) {
            if (basePtr[y * imageGray->pitch + x] == 0) {
                if (
                        basePtr[(y - 1) * imageGray->pitch + x - 1] == 255 ||
                        basePtr[(y - 1) * imageGray->pitch + x] == 255 ||
                        basePtr[(y - 1) * imageGray->pitch + x + 1] == 255 ||

                        basePtr[(y) * imageGray->pitch + x - 1] == 255 ||
                        basePtr[(y) * imageGray->pitch + x] == 255 ||
                        basePtr[(y) * imageGray->pitch + x + 1] == 255 ||

                        basePtr[(y + 1) * imageGray->pitch + x - 1] == 255 ||
                        basePtr[(y + 1) * imageGray->pitch + x] == 255 ||
                        basePtr[(y + 1) * imageGray->pitch + x + 1] == 255
                        ) {
                    basePtr2[y * imageGray2->pitch + x] = 255;
                }
            }
        }
    }
    unlockSurface(imageGray2);
    cloneSurface(&imageGray2, imageGrayPtr);
    deleteSurface(&imageGray2);
    return true;
}

bool add(SDL_Surface **imageAPtr, SDL_Surface **imageBPtr, SDL_Surface **imageCPtr) {
    SDL_Surface *imageA = *imageAPtr;
    SDL_Surface *imageB = *imageBPtr;

    if (!(imageA->w == imageB->w && imageA->h == imageB->h)) {
        return false;
    }

    SDL_Surface *imageT = NULL;
    cloneSurface(&imageA, &imageT);

    if (!lockSurface(imageA)) {
        deleteSurface(&imageT);
        return false;
    }
    if (!lockSurface(imageB)) {
        unlockSurface(imageA);
        deleteSurface(&imageT);
        return false;
    }
    if (!lockSurface(imageT)) {
        unlockSurface(imageB);
        unlockSurface(imageA);
        deleteSurface(&imageT);
        return false;
    }
    Uint8 *basePtrA = (Uint8 *) imageA->pixels;
    Uint8 *basePtrB = (Uint8 *) imageB->pixels;
    Uint8 *basePtrT = (Uint8 *) imageT->pixels;
    for (int y = 0; y != imageA->h; ++y) {
        for (int x = 0; x != imageA->w; ++x) {
            int t = (int) (basePtrA[y * imageA->pitch + x]) + (int) (basePtrB[y * imageB->pitch + x]);
            if (t > 255) {
                basePtrT[y * imageT->pitch + x] = 255;
            } else {
                basePtrT[y * imageT->pitch + x] = (Uint8) t;
            }
        }
    }
    unlockSurface(imageT);
    unlockSurface(imageB);
    unlockSurface(imageA);

    cloneSurface(&imageT, imageCPtr);
    deleteSurface(&imageT);
    return true;
}

bool sub(SDL_Surface **imageAPtr, SDL_Surface **imageBPtr, SDL_Surface **imageCPtr) {
    SDL_Surface *imageA = *imageAPtr;
    SDL_Surface *imageB = *imageBPtr;

    if (!(imageA->w == imageB->w && imageA->h == imageB->h)) {
        return false;
    }

    SDL_Surface *imageT = NULL;
    cloneSurface(&imageA, &imageT);

    if (!lockSurface(imageA)) {
        deleteSurface(&imageT);
        return false;
    }
    if (!lockSurface(imageB)) {
        unlockSurface(imageA);
        deleteSurface(&imageT);
        return false;
    }
    if (!lockSurface(imageT)) {
        unlockSurface(imageB);
        unlockSurface(imageA);
        deleteSurface(&imageT);
        return false;
    }
    Uint8 *basePtrA = (Uint8 *) imageA->pixels;
    Uint8 *basePtrB = (Uint8 *) imageB->pixels;
    Uint8 *basePtrT = (Uint8 *) imageT->pixels;
    for (int y = 0; y != imageA->h; ++y) {
        for (int x = 0; x != imageA->w; ++x) {
            int t = (int) (basePtrA[y * imageA->pitch + x]) - (int) (basePtrB[y * imageB->pitch + x]);
            if (t < 0) {
                basePtrT[y * imageT->pitch + x] = 0;
            } else {
                basePtrT[y * imageT->pitch + x] = (Uint8) t;
            }
        }
    }
    unlockSurface(imageT);
    unlockSurface(imageB);
    unlockSurface(imageA);

    cloneSurface(&imageT, imageCPtr);
    deleteSurface(&imageT);
    return true;
}

bool negative(SDL_Surface **imagePtr) {
    SDL_Surface *image = *imagePtr;
    if (!lockSurface(image)) {
        return false;
    }
    Uint8 *basePtr = (Uint8 *) image->pixels;
    for (int y = 0; y != image->h; ++y) {
        for (int x = 0; x != image->w; ++x) {
            basePtr[y * image->pitch + x] = (Uint8) 255 - basePtr[y * image->pitch + x];
        }
    }
    unlockSurface(image);
    return true;
}

bool addNumber(SDL_Surface **imagePtr, int num) {
    SDL_Surface *image = *imagePtr;
    if (!lockSurface(image)) {
        return false;
    }
    Uint8 *basePtr = (Uint8 *) image->pixels;
    for (int y = 0; y != image->h; ++y) {
        for (int x = 0; x != image->w; ++x) {
            int t = (int) (basePtr[y * image->pitch + x]) + num;
            if (t > 255) {
                basePtr[y * image->pitch + x] = 255;
            } else if (t < 0) {
                basePtr[y * image->pitch + x] = 0;
            } else {
                basePtr[y * image->pitch + x] = (Uint8) t;
            }
        }
    }
    unlockSurface(image);
    return true;
}

bool subNumber(SDL_Surface **imagePtr, int num) {
    SDL_Surface *image = *imagePtr;
    if (!lockSurface(image)) {
        return false;
    }
    Uint8 *basePtr = (Uint8 *) image->pixels;
    for (int y = 0; y != image->h; ++y) {
        for (int x = 0; x != image->w; ++x) {
            int t = (int) (basePtr[y * image->pitch + x]) - num;
            if (t > 255) {
                basePtr[y * image->pitch + x] = 255;
            } else if (t < 0) {
                basePtr[y * image->pitch + x] = 0;
            } else {
                basePtr[y * image->pitch + x] = (Uint8) t;
            }
        }
    }
    unlockSurface(image);
    return true;
}

bool mulNumber(SDL_Surface **imagePtr, int num) {
    SDL_Surface *image = *imagePtr;
    if (!lockSurface(image)) {
        return false;
    }
    Uint8 *basePtr = (Uint8 *) image->pixels;
    for (int y = 0; y != image->h; ++y) {
        for (int x = 0; x != image->w; ++x) {
            int t = (int) (basePtr[y * image->pitch + x]) * num;
            if (t > 255) {
                basePtr[y * image->pitch + x] = 255;
            } else if (t < 0) {
                basePtr[y * image->pitch + x] = 0;
            } else {
                basePtr[y * image->pitch + x] = (Uint8) t;
            }
        }
    }
    unlockSurface(image);
    return true;
}

bool divNumber(SDL_Surface **imagePtr, int num) {
    SDL_Surface *image = *imagePtr;
    if (!lockSurface(image)) {
        return false;
    }
    Uint8 *basePtr = (Uint8 *) image->pixels;
    for (int y = 0; y != image->h; ++y) {
        for (int x = 0; x != image->w; ++x) {
            int t = (int) (basePtr[y * image->pitch + x]) / num;
            if (t > 255) {
                basePtr[y * image->pitch + x] = 255;
            } else if (t < 0) {
                basePtr[y * image->pitch + x] = 0;
            } else {
                basePtr[y * image->pitch + x] = (Uint8) t;
            }
        }
    }
    unlockSurface(image);
    return true;
}

int Uint8Compare(const void *a, const void *b) {
    return (*(Uint8 *) a - *(Uint8 *) b);
}

bool medianBlur(SDL_Surface **imageGrayPtr) {
    SDL_Surface *imageGray = *imageGrayPtr;
    SDL_Surface *imageGray2 = NULL;
    cloneSurface(imageGrayPtr, &imageGray2);
    if (!lockSurface(imageGray2)) {
        deleteSurface(&imageGray2);
        return false;
    }
    assert(imageGray2);
    Uint8 *basePtr = (Uint8 *) imageGray->pixels;
    Uint8 *basePtr2 = (Uint8 *) imageGray2->pixels;
    for (int y = 1; y != imageGray2->h - 1; ++y) {
        for (int x = 1; x != imageGray2->w - 1; ++x) {
            Uint8 box[9];
            box[0] = basePtr[(y - 1) * imageGray->pitch + x - 1];
            box[1] = basePtr[(y - 1) * imageGray->pitch + x];
            box[2] = basePtr[(y - 1) * imageGray->pitch + x + 1];
            box[3] = basePtr[(y) * imageGray->pitch + x - 1];
            box[4] = basePtr[(y) * imageGray->pitch + x];
            box[5] = basePtr[(y) * imageGray->pitch + x + 1];
            box[6] = basePtr[(y + 1) * imageGray->pitch + x - 1];
            box[7] = basePtr[(y + 1) * imageGray->pitch + x];
            box[8] = basePtr[(y + 1) * imageGray->pitch + x + 1];
            qsort(box, 9, sizeof(Uint8), Uint8Compare);
            basePtr2[y * imageGray2->pitch + x] = box[4];
        }
    }
    unlockSurface(imageGray2);
    cloneSurface(&imageGray2, imageGrayPtr);
    deleteSurface(&imageGray2);
    return true;
}

bool medianBlurN(SDL_Surface **imageGrayPtr, int xN, int yN) {
    assert(xN >= 3 && yN >= 3 && xN % 2 == 1 && yN % 2 == 1);
    int shiftX = (xN - 1) / 2;
    int shiftY = (yN - 1) / 2;
    SDL_Surface *imageGray = *imageGrayPtr;
    SDL_Surface *imageGray2 = NULL;
    cloneSurface(imageGrayPtr, &imageGray2);
    if (!lockSurface(imageGray2)) {
        deleteSurface(&imageGray2);
        return false;
    }
    assert(imageGray2);
    Uint8 *basePtr = (Uint8 *) imageGray->pixels;
    Uint8 *basePtr2 = (Uint8 *) imageGray2->pixels;
    Uint8 *box = (Uint8 *) malloc(xN * xN * sizeof(Uint8));
    for (int y = shiftY; y != imageGray2->h - shiftY; ++y) {
        for (int x = shiftX; x != imageGray2->w - shiftX; ++x) {
            int c = 0;
            for (int yP = y - shiftY; yP != y + shiftY + 1; ++yP) {
                for (int xP = x - shiftX; xP != x + shiftX + 1; ++xP) {
                    box[c] = basePtr[yP * imageGray->pitch + xP];
                    ++c;
                }
            }
            assert(c == xN * xN);
            qsort(box, (size_t) (xN * xN), sizeof(Uint8), Uint8Compare);
            basePtr2[y * imageGray2->pitch + x] = box[(xN * xN - 1) / 2 + 1];
        }
    }
    free(box);
    box = NULL;
    unlockSurface(imageGray2);
    cloneSurface(&imageGray2, imageGrayPtr);
    deleteSurface(&imageGray2);
    return true;
}


