#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

#include "global_header.h"

#define FLAG_PUBLIC
#define FLAG_PRIVATE


int main(int argc, char *argv[]) {
    printf("Hello, World!\n");

    // 初始化图像环境
    if (!initEverything()) {
        printf_s("init failed.");
        return (1);
    }

    // 初始化显示窗口  并设置初始窗口大小为 宽800像素 高600像素
    if (initWindow("SDL Tutorial", 800, 600) != true) {
        printf_s("window create failed.");
        return (1);
    }

    // 从图片文件读取图像，在图像左上角画一个矩形，显示并保存图像
    {
        // 创建一个用来持有图像的图像表面指针
        SDL_Surface *image;
        // 从图片文件读取图像表面并由image持有
        if (!loadImage("lena.png", &image)) {
            printf_s("image load failed.");
            return (1);
        }

        // 打出显示图像表面的宽和高
        printf_s("w:%d h:%d \n", image->w, image->h);

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

        printf_s("end.");
        waitKey(0);
        deleteSurface(&picture);
    }

    // 关闭图像环境
    closeEverything();

    return 0;
}

