#ifndef COLORS_H_INCLUDED
#define COLORS_H_INCLUDED

#include <SDL2/SDL.h>

/**
* Adds color to the palette of colors.
* @param color - SDL_Color structure instance specifying R, G, B values of the added color.
* @return int value which specifies position of added color in "palette" array.
*/
int addColor(SDL_Color color);

/**
* Compares given two colors.
* @param firstColor - SDL_Color structure instance which represents first color to be compared.
* @param secondColor - SDL_Color structure instance which represents second color to be compared.
* @return true if the colors are the same, false otherwise.
*/
bool compareColors(SDL_Color firstColor, SDL_Color secondColor);

/**
* Checks if given color is in the palette. If no it invokes addColor() function which adds this color
* to the palette.
* @param color - SDL_Color structure instance representing color to be checked.
* @return position of color in the "palette" array.
*/
int checkColor(SDL_Color color);

/**
* Normalizes the given value in the range of 0 to 255.
* @param value - floating point variable representing value to be normalized.
* @return int value after normalization
*/
int normalization(float value);

#endif // COLORS_H_INCLUDED
