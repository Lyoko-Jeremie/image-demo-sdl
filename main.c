#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

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

    {
        // 创建一个用来持有图像的图像表面指针
        SDL_Surface *picture;

        // 创建一个宽800高600的新的黑色的图像表面并由picture持有
        if (!createSurface(800, 600, &picture)) {
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

        // 使用黑色清空窗口
        clearWindowWithBlack();
        // 把图像表面picture持有的图像绘制到窗口上
        drawImageToWindow(picture);

        waitKey(0);
        // 销毁picture持有的图像表面
        deleteSurface(&picture);
    }

    // 关闭图像环境
    closeEverything();

    return 0;
}

