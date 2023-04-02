#ifndef VIEWPORT_H_INCLUDED
#define VIEWPORT_H_INCLUDED

#include <SDL2/SDL.h>

/**
* Sets the color of the given pixel on the viewport.
* @param x - x position of the pixel
* @param y - y position of the pixel
* @param R - R component of the color to be set
* @param G - G component of the color to be set
* @param B - B component of the color to be set
*/
void setPixel(int x, int y, Uint8 R, Uint8 G, Uint8 B);

/**
* Gets the color of the specified pixel.
* @param x - x position of the pixel
* @param y - y position of the pixel
* @return SDL_Color structure instance which contains information about
* pixel's color
*/
SDL_Color getPixel(int x, int y);

/**
* Gets the color of the specified pixel from the given surface.
* @param x - x position of the pixel
* @param y - y position of the pixel
* @param surface - pointer to SDL_Surface which represents the surface from which
* we want to get pixel
* @return SDL_Color structure instance which contains information about
* pixel's color
*/
SDL_Color getPixelSurface(int x, int y, SDL_Surface* surface);

/**
* Loads the BMP file and views its data on the viewport.
* @param name - name of the BMP file
*/
void loadBMP(char const* name);

/**
* Clears the screen by setting all pixels to specified color.
* @param R - R component of the specified color
* @param G - G component of the specified color
* @param B - B component of the specified color
*/
void clearScreen(Uint8 R, Uint8 G, Uint8 B);



#endif // VIEWPORT_H_INCLUDED
