cmake_minimum_required(VERSION 3.8)
project(demo_sdl)

set(CMAKE_C_STANDARD 11)

include_directories(
        "${CMAKE_SOURCE_DIR}/SDL/SDL2-2.0.7/x86_64-w64-mingw32/include/SDL2"
        "${CMAKE_SOURCE_DIR}/SDL/SDL2_image-2.0.2/x86_64-w64-mingw32/include/SDL2"
)

link_directories(
        "${CMAKE_SOURCE_DIR}/SDL/SDL2-2.0.7/x86_64-w64-mingw32/lib"
        "${CMAKE_SOURCE_DIR}/SDL/SDL2_image-2.0.2/x86_64-w64-mingw32/lib"
)

link_libraries(
        SDL2
        SDL2main
        SDL2_image
)

set(SOURCE_FILES main.c imgproc.c imgproc.h highgui.h highgui.c)
add_executable(demo_sdl ${SOURCE_FILES})