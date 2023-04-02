#include <iostream>
#include <SDL2/SDL.h>
#include "ConversionFunctions.h"
#include "ByteRun.h"
#include "Colors.h"
#include "Image.h"
#include "MedianCut.h"
#include "Viewport.h"

SDL_Window* window = NULL;
SDL_Surface* screen = NULL;

#define WINDOW_TITLE "Converter"

#define WIDTH 512
#define HEIGHT 340

void viewMenu(){
    std::cout << "a-f: load image to convert\nImages must be 256x170 and named imagex.bmp where x can be 1, 2, 3, 4, 5, 6, 7 \n\n";

    std::cout << "1. Save the image in grayscale\n";
    std::cout << "2. Save the image using the imposed palette\n";
    std::cout << "3. Save the image using the dedicated palette\n\n";

    std::cout << "4. Save the image in grayscale with dithering\n";
    std::cout << "5. Save the image using the imposed palette with dithering\n";
    std::cout << "6. Save the image using the dedicated palette with dithering\n\n";

    std::cout << "7. Load saved image\n\n";
}

void threeBitBW() {
    saveImage_3bit_BW(false);
    SDL_UpdateWindowSurface(window);
}

void threeBitImposedPalette() {
    saveImage_threeBitImposedPalette(false);
    SDL_UpdateWindowSurface(window);
}

void threeBitDedicatedPalette() {
    createDedicatedPalette();
    saveImage_threeBitDedicatedPalette(false);
    clear_dedicatedPaletteColorCount();
    SDL_UpdateWindowSurface(window);
}

void threeBitBWwithDithering() {
    saveImage_3bit_BW(true);
    SDL_UpdateWindowSurface(window);
}

void threeBitImposedPaletteWithDithering() {
    saveImage_threeBitImposedPalette(true);
    SDL_UpdateWindowSurface(window);
}

void threeBitDedicatedPaletteWithDithering() {
    createDedicatedPalette();
    saveImage_threeBitDedicatedPalette(true);
    clear_dedicatedPaletteColorCount();
    SDL_UpdateWindowSurface(window);
}

void loadConvertedImage() {
    loadImage();
    SDL_UpdateWindowSurface(window);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2, HEIGHT * 2, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    screen = SDL_GetWindowSurface(window);
    if (screen == NULL) {
        fprintf(stderr, "SDL_GetWindowSurface Error: %s\n", SDL_GetError());
        return false;
    }
    SDL_UpdateWindowSurface(window);


    bool done = false;
    SDL_Event event;
    viewMenu();

    //main loop of the program
    while (SDL_WaitEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            done = true;
            break;

            //check if button was pressed
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE)
                done = true;
            if (event.key.keysym.sym == SDLK_1)
                threeBitBW();
            if (event.key.keysym.sym == SDLK_2)
                threeBitImposedPalette();
            if (event.key.keysym.sym == SDLK_3)
                threeBitDedicatedPalette();
            if (event.key.keysym.sym == SDLK_4)
                threeBitBWwithDithering();
            if (event.key.keysym.sym == SDLK_5)
                threeBitImposedPaletteWithDithering();
            if (event.key.keysym.sym == SDLK_6)
                threeBitDedicatedPaletteWithDithering();
            if (event.key.keysym.sym == SDLK_7)
                loadConvertedImage();
            if (event.key.keysym.sym == SDLK_a)
                loadBMP("image1.bmp");
            if (event.key.keysym.sym == SDLK_s)
                loadBMP("image2.bmp");
            if (event.key.keysym.sym == SDLK_d)
                loadBMP("image3.bmp");
            if (event.key.keysym.sym == SDLK_f)
                loadBMP("image4.bmp");
            if (event.key.keysym.sym == SDLK_g)
                loadBMP("image5.bmp");
            if (event.key.keysym.sym == SDLK_h)
                loadBMP("image6.bmp");
            if (event.key.keysym.sym == SDLK_j)
                loadBMP("image7.bmp");
            if (event.key.keysym.sym == SDLK_b)
                clearScreen(0, 0, 10);
            else
                break;
        }
        }
        if (done)
            break;
    }

    if (screen) {
        SDL_FreeSurface(screen);
    }

    if (window) {
        SDL_DestroyWindow(window);
    }


    SDL_Quit();
    return 0;
}


