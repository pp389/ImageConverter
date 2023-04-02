#include <cmath>
#include "ConversionFunctions.h"


Uint8 from_24RGB_to_3RGB(SDL_Color color) {
    int R = color.r, G = color.g, B = color.b, newR, newG, newB;

    newR = round(R / 255.0);
    newG = round(G / 255.0);
    newB = round(B / 255.0);

    return ((newR << 2) + (newG << 1) + (newB));
}

SDL_Color from_3RGB_to_24RGB(Uint8 color) {

    SDL_Color convertedColor;

    Uint8 R = color << 5;
    R = R >> 7;
    Uint8 G = color << 6;
    G = G >> 7;
    Uint8 B = color << 7;
    B = B >> 7;

    convertedColor.r = R * 255;
    convertedColor.g = G * 255;
    convertedColor.b = B * 255;

    return convertedColor;
}


Uint8 from_24RGB_to_3BW(SDL_Color color) {
    int R = color.r, G = color.g, B = color.b;
    Uint8 BW8bit, BW3bit;

    BW8bit = 0.299 * R + 0.587 * G + 0.114 * B;
    BW3bit = round(BW8bit * 7.0 / 255.0);

    return BW3bit;

}

SDL_Color from_3BW_to_24RGB(Uint8 color) {
    SDL_Color convertedColor;
    Uint8 BW8bit = color * 255.0 / 7.0;

    convertedColor.r = BW8bit;
    convertedColor.g = BW8bit;
    convertedColor.b = BW8bit;

    return convertedColor;
}

Uint8 from_24RGB_to_4BW(SDL_Color color) {
    Uint8 BW4bit = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
    BW4bit *= 15.0 / 255.0;
    return BW4bit;
}

SDL_Color from_4BW_to_24RGB(Uint8 color) {
    SDL_Color convertedColor;
    Uint8 BW8bit = color * 255.0 / 15.0;

    convertedColor.r = BW8bit;
    convertedColor.g = BW8bit;
    convertedColor.b = BW8bit;

    return convertedColor;
}

