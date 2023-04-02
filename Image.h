#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

#define WIDTH 512
#define HEIGHT 340

struct RGBerror {
    float r;
    float g;
    float b;
};

struct ThreeBytes {
    Uint8 firstByte, secondByte, thirdByte;
};

struct EightBytes {
    Uint8* bytes = new Uint8[8];
};

/**
* Used to draw colors from given palette.
* @param px - start point x coordinate
* @param py - start point y coordinate
* @param palette - SDL_Color array which contains palette to draw
*/
void drawPalette(int px, int py, SDL_Color palette[]);

/**
* Finds color in the given palette which is closest to other color (neighbour) specified by the argument.
* @param color - color which neighbour in the palette we are searching for
* @param palette -  SDL_Color array which contains palette in which search is performed
* @return index of the neighbout in the 'palette' array
*/
int findNeighbour(SDL_Color color, SDL_Color palette[]);

/**
* Creates palette dedicated to image which was loaded from BMP file.
*/
void createDedicatedPalette();

/**
* Saves header with information about converted image to specified file which will contain the converted image.
* @param type - char array which contains information about type of image saved in file. "BW" means image saved
* in grayscale. "DD" means that image was saved with dedicated palette. "NN" means that image was converted to
* imposed palette.
* @param compression - if equal to 'T' - image is compressed, if equal to 'N' image is not compressed
* @param dithering - if equal to 'T' - image uses dithering, if equal to 'N' image does not use dithering
*/
void saveHeaderToFile(ofstream& file, char type[], char compression, char dithering);

/**
* Loads converted image which was saved to file 'image.bin' and views in on the screen.
*/
void loadImage();

/**
* Converts image which was loaded to program to 3-bit grayscale image and saves it
* to file 'image.bin'.
* @param dithering - if equal to 'T' - use dithering, if equal to 'N' do not use dithering
*/
void saveImage_3bit_BW(bool dithering);

/**
* Converts image which was loaded to program to new image in which colors are from imposed palette and saves it
* to file 'image.bin'.
* @param dithering - if equal to 'T' - use dithering, if equal to 'N' do not use dithering
*/
void saveImage_threeBitImposedPalette(bool dithering);

/**
* Converts image which was loaded to program to new image in which colors are from palette dedicated to this image
* (palette is being created in other function) and saves this image to file 'image.bin'.
* to file 'image.bin'.
* @param dithering - if equal to 'T' - use dithering, if equal to 'N' do not use dithering
*/
void saveImage_threeBitDedicatedPalette(bool dithering);

/**
* Converted image uses only 3 bits in each of its bytes to describe a color of a pixel. This function can save information
* from the eight bytes of the image to only three bytes without losing any information about colors of the pixels.
* @param dataToCompress - pointer to an array which contains data of converted image and must be compressed
* @param length - size of 'dataToCompress' array
* @return pointer to a char array which contains data after compression
*/
char* compressFrom8to3Bytes(Uint8* dataToCompress, int length);

/**
* The opposite operation to that from function "compressFrom8to3Bytes".
* @param length - size of 'dataToCompress' array
* @param dataToCompress - pointer to an array which contains data to decompress
* @return std::vector which contains decompressed data of an image
*/
template <typename T>
std::vector<char> decompressFrom3To8Bytes(int length, T dataToDecompress);

/**
* Helper function to the 'compressFrom8to3Bytes'. Uses bitwise operations to convert 'pack' useful bits in less number of
* bytes.
* @param eightBytes - pointer to an array which should contain 8 bytes
* @return ThreeBytes structure instance which contains data about three bytes
*/
ThreeBytes convertFrom8to3Bytes(Uint8* eightBytes);

/**
* Helper function to the 'decompressFrom3To8Bytes'. Uses bitwise operations to 'unpack' useful bits to greater number of
* bytes.
* @param threeBytes - pointer to an array which should contain 3 bytes.
* @return EightBytes structure instance which contains data about eight bytes
*/
EightBytes convertFrom3to8Bytes(Uint8* threeBytes);

/**
* Sets dedicatedPaletteColorCount to zero.
*/
void clear_dedicatedPaletteColorCount();

/**
* Increments dedicatedPaletteColorCount.
*/
void increment_dedicatedPaletteColorCount();

/**
* @return value of dedicatedPaletteColorCount.
*/
int get_dedicatedPaletteColorCount();

#endif // IMAGE_H
