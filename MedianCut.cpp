#include <iostream>
#include "MedianCut.h"
#include "Image.h"

extern SDL_Color image[(WIDTH / 2) * (HEIGHT / 2)];
extern SDL_Color threeBitDedicatedPalette[8];

void medianCutRGB(int start, int end, int iteration) {
    if (iteration > 0) {
        //Firstly we sort bucket, then we divide it into two smaller ones
        //It is necessary to determine on which component to sort the bucket
        Uint8 whichComponent = greatestDifference(start,end);
        sortBucket(start, end, whichComponent);

        std::cout << "Dividing the bucket on the level: " << iteration << std::endl;
        int mid = (start + end) / 2;
        medianCutRGB(start, mid, iteration - 1);
        medianCutRGB(mid + 1, end, iteration - 1);
        std::cout << "Completed dividing on the level: " << iteration << std::endl;
    }
    else {
        //build the palette by averaging the colors from a specific bucket
        int sumR = 0, sumG = 0, sumB = 0;

        for (int p = start; p < end; p++) {
            sumR += image[p].r;
            sumG += image[p].g;
            sumB += image[p].b;
        }

        Uint8 R = sumR / (end - start), G = sumG / (end - start), B = sumB / (end - start);
        SDL_Color newColor = { R, G, B };
        threeBitDedicatedPalette[get_dedicatedPaletteColorCount()] = newColor;
        std::cout << get_dedicatedPaletteColorCount() << ": [" << (int)newColor.r << "," << (int)newColor.g << "," << (int)newColor.b << "]";
        increment_dedicatedPaletteColorCount();
    }
}

Uint8 greatestDifference(int start, int end) {
    Uint8 minRed, minBlue, minGreen, maxRed, maxBlue, maxGreen;
    minRed = minBlue = minGreen = 257;
    maxRed = maxBlue = maxGreen = 0;

    for(int i = start; i < end; i++) {
        minRed = std::min(minRed, image[i].r);
        minBlue = std::min(minBlue, image[i].b);
        minGreen = std::min(minGreen, image[i].g);

        maxRed  = std::max(maxRed, image[i].r);
        maxBlue = std::max(maxBlue, image[i].b);
        maxGreen = std::max(maxGreen, image[i].g);
   }

    Uint8 red = maxRed - minRed, green = maxGreen - minGreen, blue = maxBlue - minBlue,
                max = std::max(std::max(red, green), blue);

    if(max == red)
        return 1;
    else if(max == green)
        return 2;
    else if(max == blue)
       return 3;
}

void sortBucket(int start, int end, Uint8 whichComponent){
    int maximum = start;
    for (int p = start; p < end; p++) {
        maximum = p;
        for (int i = p; i < end; i++) {

                switch(whichComponent){
                case 1:
                    if (image[i].r > image[maximum].r)
                       maximum = i;
                       break;
                case 2:
                    if (image[i].g > image[maximum].g)
                       maximum = i;
                       break;
                case 3:
                    if (image[i].b > image[maximum].b)
                       maximum = i;
                       break;
                default:
                    break;
                }

        }
        std::swap(image[p], image[maximum]);
    }
}
