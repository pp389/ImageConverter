#ifndef BYTERUN_H_INCLUDED
#define BYTERUN_H_INCLUDED

#include <vector>

/**
* Performs decompression of data previously compressed by ByteRun algorithm.
* @param input - char array which contains data to be decompressed
* @param length - int value representing length of the "input" array
* @return std::vector which contains decompressed data
*/
std::vector<char> ByteRunDecompression(char input[], int length);

/**
* Performs lossless compression of given data by the ByteRun algorithm.
* @param input - char array which contains data to be compressed
* @param length - int value representing length of the "input" array
* @return std::vector which contains compressed data
*/
std::vector<char> ByteRunCompression(char input[], int length);


#endif // BYTERUN_H_INCLUDED
