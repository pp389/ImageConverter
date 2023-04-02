#ifndef MEDIANCUT_H_INCLUDED
#define MEDIANCUT_H_INCLUDED

#include <SDL2/SDL.h>

/**
* Implementation of median cut algorithm for RGB palette.
* @param start - number of first pixel (index of the "image" array)
* @param end - number of last pixel (index of the "image" array)
* @param iteration - number of iterations to divide bucket
*/
void medianCutRGB(int start, int end, int iteration);

/**
* Finds which color component has the greatest difference in given sequence
* of pixels.
* @param start - number of first pixel (index of the "image" array)
* @param end - number of last pixel (index of the "image" array)
* @return 8-bit unsigned int value
*/
Uint8 greatestDifference(int start, int end);

/**
* Used to sort given bucket of pixels.
* @param start - number of first pixel in the bucket (index of the "image" array)
* @param end - number of last pixel int the bucket (index of the "image" array)
* @param whichComponent - specifies by which component of color (R, G or B) we want to sort bucket
*/
void sortBucket(int start, int end, Uint8 whichComponent);


#endif // MEDIANCUT_H_INCLUDED
