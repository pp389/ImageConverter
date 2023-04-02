#ifndef CONVERSIONFUNCTIONS_H_INCLUDED
#define CONVERSIONFUNCTIONS_H_INCLUDED

#include <SDL2/SDL.h>

/**
* Converts given 24-bit RGB color to 3-bit RGB.
* @param color - SDL_Color instance which contains data of color to be converted.
* @return 8-bit unsigned int value which represents 3-bit RGB color after conversion.
*/
Uint8 from_24RGB_to_3RGB(SDL_Color color);

/**
* Converts given 3-bit RGB color to 24-bit RGB.
* @param color - 8-bit unsigned int value which represents 3-bit RGB color to be converted.
* @return SDL_Color instance which contains data of color after conversion.
*/
SDL_Color from_3RGB_to_24RGB(Uint8 color);

/**
* Converts given 24-bit RGB color to 3-bit BW.
* @param color - SDL_Color instance which contains data of color to be converted.
* @return 8-bit unsigned int value which represents 3-bit BW color after conversion.
*/
Uint8 from_24RGB_to_3BW(SDL_Color color);

/**
* Converts given 3-bit BW color to 24-bit RGB.
* @param color - 8-bit unsigned int value which represents 3-bit BW color to be converted.
* @return SDL_Color instance which contains data of color after conversion.
*/
SDL_Color from_3BW_to_24RGB(Uint8 color);

/**
* Converts given 24-bit RGB color to 4-bit BW.
* @param color - SDL_Color instance which contains data of color to be converted.
* @return 8-bit unsigned int value which represents 4-bit BW color after conversion.
*/
Uint8 from_24RGB_to_4BW(SDL_Color color);

/**
* Converts given 4-bit BW color to 24-bit RGB.
* @param color - 8-bit unsigned int value which represents 4-bit BW color to be converted.
* @return SDL_Color instance which contains data of color after conversion.
*/
SDL_Color from_4BW_to_24RGB(Uint8 color);


#endif // CONVERSIONFUNCTIONS_H_INCLUDED
