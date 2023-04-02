#include <iostream>
#include "Colors.h"

int numberOfColors = 0;
SDL_Color palette[43520];

int addColor(SDL_Color color) {
    int currentColor = numberOfColors;
    palette[currentColor] = color;
    std::cout << currentColor << ": [" << (int)color.r << ", " << (int)color.g << ", " << (int)color.b << "]" << std::endl;
    numberOfColors++;
    return currentColor;
}

bool compareColors(SDL_Color firstColor, SDL_Color secondColor) {
    if ((firstColor.r != secondColor.r) || (firstColor.g != secondColor.g) || (firstColor.b != secondColor.b))
        return false;

    return true;
}

int checkColor(SDL_Color color) {
    if (numberOfColors > 0) {
        for (int k = 0; k < numberOfColors; k++) {
            if (compareColors(color, palette[k]))
                return k;
        }
    }

    return addColor(color);
}

int normalization(float value) {
    if (value > 255)
        return 255;
    if(value < 0)
        return 0;

    return value;
}
