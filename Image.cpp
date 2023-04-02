#include <cstdlib>
#include <cstring>
#include "Image.h"
#include "Viewport.h"
#include "ConversionFunctions.h"
#include "MedianCut.h"
#include "Colors.h"
#include "ByteRun.h"

SDL_Color threeBitImposedPalette[8];
SDL_Color threeBitDedicatedPalette[8];
SDL_Color image[(WIDTH / 2) * (HEIGHT / 2)];

int dedicatedPaletteColorCount = 0;

extern SDL_Window* window;
extern SDL_Surface* screen;


void drawPalette(int px, int py, SDL_Color palette[]) {

    int addX = WIDTH / 2 / 4, addY = 0, xx = 0, yy = HEIGHT / 2 / 2;

    for (int x = 0; x < 8; x++) {
        if ((x + 1) % 5 == 0) {
            xx = 0;
            addX = WIDTH / 2 / 4;
            addY += HEIGHT / 2 / 2;
            yy += HEIGHT / 2 / 2;
        }
        for (; xx < addX; xx++) {
            for (int y = addY; y < yy; y++)
                setPixel(xx + px, y + py, palette[x].r, palette[x].g, palette[x].b);
        }
        addX += WIDTH / 2 / 4;
    }

}

int findNeighbour(SDL_Color color, SDL_Color palette[]) {
    int minimum = 999, indexMinimum = 0, distance;
    SDL_Color colorFromPalette;

    for (int i = 0; i < 8; i++) {
        colorFromPalette = palette[i];
        distance = abs(color.r - colorFromPalette.r) + abs(color.g - colorFromPalette.g) + abs(color.b - colorFromPalette.b);

        if (distance < minimum) {
            indexMinimum = i;
            minimum = distance;
        }
    }

    return indexMinimum;
}

void createDedicatedPalette() {
    SDL_Color color;
    int numberOfPixels = 0;

    for (int y = 0; y < HEIGHT / 2; y++) {
        for (int x = 0; x < WIDTH / 2; x++) {
            color = getPixel(x, y);
            image[numberOfPixels] = { color.r, color.g, color.b };
            numberOfPixels++;
        }
    }

    SDL_UpdateWindowSurface(window);

    int newColor;
    for (int y = 0; y < HEIGHT / 2; y++) {
        for (int x = 0; x < WIDTH / 2; x++) {
            color = getPixel(x, y);
            newColor = from_24RGB_to_3RGB(color);
            color = from_3RGB_to_24RGB(newColor);
        }
    }

    medianCutRGB(0, numberOfPixels, 3);
    drawPalette(WIDTH / 2, HEIGHT / 2 , threeBitDedicatedPalette);
}

void saveHeaderToFile(ofstream& file, char type[], char compression, char dithering) {
    Uint16 imageWidth = WIDTH/2, imageHeight = HEIGHT/2;
    char identifier[] = "PP";
    file.write((char*)&identifier, sizeof(char)*2);
    file.write((char*)&type[0], sizeof(char));
    file.write((char*)&type[1], sizeof(char));
    file.write((char*)&imageWidth, sizeof(Uint16));
    file.write((char*)&imageHeight, sizeof(Uint16));
    file.write((char*)&dithering, sizeof(char));
}

void loadImage() {

    SDL_Color color;
    Uint16 imageWidth = 0, imageHeight = 0;
    char identifier[2], type[2], compression, dithering;
    std::ifstream input("image.bin" , ios::binary);

    std::cout << "Loading file 'image.bin'." << std::endl;

    input.read((char*)&identifier, sizeof(char)*2);
    input.read((char*)&type, sizeof(char)*2);
    input.read((char*)&imageWidth, sizeof(Uint16));
    input.read((char*)&imageHeight, sizeof(Uint16));
    input.read((char*)&dithering, sizeof(char));

    char* values = new char[imageWidth * imageHeight];
    int j = 0;

    if(type[0] == 'D' && type[1] == 'D') {
        for(int i = 0; i < 8; i++) {
            input.read((char*)&threeBitDedicatedPalette[i].r , sizeof(Uint8));
            input.read((char*)&threeBitDedicatedPalette[i].g , sizeof(Uint8));
            input.read((char*)&threeBitDedicatedPalette[i].b , sizeof(Uint8));
        }

        input.read(values,sizeof(char)*imageWidth*imageHeight);

        vector<char> decompressed = ByteRunDecompression(values, imageHeight*imageWidth);
        decompressed = decompressFrom3To8Bytes(imageWidth*imageHeight, decompressed);

        for(int y = 0; y < imageHeight; y++) {
            for(int x = 0; x < imageWidth; x++){
                setPixel(x + WIDTH/2, y, threeBitDedicatedPalette[decompressed[j]].r, threeBitDedicatedPalette[decompressed[j]].g, threeBitDedicatedPalette[decompressed[j]].b);
                j++;
            }
        }

        input.close();
        return;
    }

    input.read(values, sizeof(char) * imageWidth * imageHeight);
    vector<char> decompressed = ByteRunDecompression(values, imageHeight * imageWidth);
    decompressed = decompressFrom3To8Bytes(imageWidth * imageHeight, decompressed);


    for(int y = 0; y < imageHeight; y++) {
        for(int x = 0; x < imageWidth; x++) {
            if(type[0] == 'B' && type[1] == 'W')
                color = from_3BW_to_24RGB(decompressed[j]);
            else if(type[0] == 'N' && type[1] == 'N')
                color = from_3RGB_to_24RGB(decompressed[j]);

            setPixel(x+imageWidth, y, color.r, color.g, color.b);
            j++;
        }
    }

    input.close();
}


void saveImage_3bit_BW(bool dithering) {
    int BW, newBW, offset = 1;
    float errors[(WIDTH / 2) + 2][(HEIGHT / 2) + 1], error = 0;
    memset(errors, 0, sizeof(errors));

    Uint8* values = new Uint8[(HEIGHT / 2) * (WIDTH / 2)];

    std::cout << "Saving file 'image.bin'." << std::endl;

    ofstream output("image.bin", ios::binary);

    if(dithering)
        saveHeaderToFile(output, "BW", 'T' ,'T');
    else
        saveHeaderToFile(output, "BW", 'T' ,'N');

    int j = 0;
    for(int y = 0; y < HEIGHT / 2; y++) {
        for(int x = 0; x < WIDTH / 2; x++) {
            SDL_Color color = getPixel(x,y);
            if(dithering) {
                BW = 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
                BW += errors[x + offset][y];
                BW = normalization(BW);

                newBW = from_24RGB_to_3BW({BW, BW, BW});
                newBW = from_3BW_to_24RGB(newBW).r;
                error = BW - newBW;

                errors[x+offset + 1][y] += error * 7.0 / 16.0;
                errors[x+offset + 1][y + 1] += error * 1.0 / 16.0;
                errors[x+offset][y + 1] += error * 5.0 / 16.0;
                errors[x+offset-1][y + 1] += error * 3.0 / 16.0;

                values[j] = from_24RGB_to_3BW({BW,BW,BW});
            }
            else
                values[j] = from_24RGB_to_3BW(color);
            j++;
        }
     }

     char* compressed = compressFrom8to3Bytes(values, (WIDTH / 2)*(HEIGHT / 2));
     vector<char> temp = ByteRunCompression(compressed, (WIDTH / 2) * (HEIGHT / 2)*3/8);

     for(int i = 0; i < temp.size(); i++)
        output.write(&temp[i], sizeof(char));

     output.close();
}

void saveImage_threeBitImposedPalette(bool dithering) {
    int R, G, B, offset = 1;
    RGBerror errors[(WIDTH / 2) + 2][(HEIGHT / 2) + 1];
    memset(errors, 0, sizeof(errors));
    Uint8* values = new Uint8[(HEIGHT / 2)*(WIDTH / 2)];
    cout<<"Saving file 'image.bin'."<<endl;

    ofstream output("image.bin" , ios::binary);

    if(dithering)
        saveHeaderToFile(output, "NN", 'T' ,'T');
    else
        saveHeaderToFile(output, "NN", 'T' ,'N');

    int j = 0;
    for(int y = 0; y < HEIGHT / 2; y++) {
        for(int x = 0; x < WIDTH / 2; x++) {
            SDL_Color color = getPixel(x,y);
            if(dithering){
                R = color.r; G = color.g; B = color.b;

                R += errors[x + offset][y].r;
                G += errors[x + offset][y].g;
                B += errors[x + offset][y].b;

                R = normalization(R); G = normalization(G); B = normalization(B);

                float errorR = R - from_3RGB_to_24RGB(from_24RGB_to_3RGB({R, G, B})).r,
                errorG = G - from_3RGB_to_24RGB(from_24RGB_to_3RGB({R, G, B})).g,
                errorB = B - from_3RGB_to_24RGB(from_24RGB_to_3RGB({R, G, B})).b;

                errors[x + offset + 1][y].r += errorR * 7.0 / 16.0;
                errors[x + offset + 1][y + 1].r += errorR * 1.0 / 16.0;
                errors[x + offset][y + 1].r += errorR * 5.0 / 16.0;
                errors[x + offset - 1][y + 1].r += errorR * 3.0 / 16.0;

                errors[x + offset + 1][y].g += errorG * 7.0 / 16.0;
                errors[x + offset + 1][y + 1].g += errorG * 1.0 / 16.0;
                errors[x + offset][y + 1].g += errorG * 5.0 / 16.0;
                errors[x + offset - 1][y + 1].g += errorG * 3.0 / 16.0;

                errors[x + offset + 1][y].b += errorB * 7.0 / 16.0;
                errors[x + offset + 1][y + 1].b += errorB * 1.0 / 16.0;
                errors[x + offset][y + 1].b += errorB * 5.0 / 16.0;
                errors[x + offset - 1][y + 1].b += errorB * 3.0 / 16.0;

                values[j] = from_24RGB_to_3RGB({R, G, B});
            }
            else{
                values[j] = from_24RGB_to_3RGB(color);
            }
            j++;
        }
     }
     char* compressed = compressFrom8to3Bytes(values, (WIDTH / 2) * (HEIGHT / 2));
     vector<char> temp = ByteRunCompression(compressed, (WIDTH / 2) * (HEIGHT / 2) * 3 / 8);

     for(int i = 0; i < temp.size(); i++){
         output.write(&temp[i], sizeof(char));
     }

    output.close();
}

void saveImage_threeBitDedicatedPalette(bool dithering) {
    int R, G, B, offset = 1;
    RGBerror errors[(WIDTH / 2) + 2][(HEIGHT / 2) + 1];
    memset(errors, 0, sizeof(errors));

    Uint8* values = new Uint8[(HEIGHT/2)*(WIDTH/2)];
    cout<<"Saving file 'image.bin'."<<endl;
    ofstream output("image.bin" , ios::binary);

    if(dithering)
        saveHeaderToFile(output, "DD", 'T' ,'T');
    else
        saveHeaderToFile(output, "DD", 'T' ,'N');

    int j = 0;

    for(int i = 0 ; i < 8 ; i++) {
        output.write((char*)&threeBitDedicatedPalette[i].r , sizeof(Uint8));
        output.write((char*)&threeBitDedicatedPalette[i].g , sizeof(Uint8));
        output.write((char*)&threeBitDedicatedPalette[i].b , sizeof(Uint8));
    }

    for(int y = 0; y < HEIGHT / 2; y++) {
        for(int x = 0; x < WIDTH / 2; x++) {
            SDL_Color color = getPixel(x,y);
             if(dithering){
                R = color.r; G = color.g; B = color.b;

                R += errors[x + offset][y].r;
                G += errors[x + offset][y].g;
                B += errors[x + offset][y].b;

                R = normalization(R); G = normalization(G); B = normalization(B);

                float errorR = R - threeBitDedicatedPalette[findNeighbour({R, G, B}, threeBitDedicatedPalette)].r,
                errorG = G - threeBitDedicatedPalette[findNeighbour({R, G, B}, threeBitDedicatedPalette)].g,
                errorB = B - threeBitDedicatedPalette[findNeighbour({R, G, B}, threeBitDedicatedPalette)].b;

                errors[x + offset + 1][y].r += errorR * 7.0 / 16.0;
                errors[x + offset + 1][y + 1].r += errorR * 1.0 / 16.0;
                errors[x + offset][y + 1].r += errorR * 5.0 / 16.0;
                errors[x + offset - 1][y + 1].r += errorR * 3.0 / 16.0;

                errors[x + offset + 1][y].g += errorG * 7.0 / 16.0;
                errors[x + offset + 1][y + 1].g += errorG * 1.0 / 16.0;
                errors[x + offset][y + 1].g += errorG * 5.0 / 16.0;
                errors[x + offset - 1][y + 1].g += errorG * 3.0 / 16.0;

                errors[x + offset + 1][y].b += errorB * 7.0 / 16.0;
                errors[x + offset + 1][y + 1].b += errorB * 1.0 / 16.0;
                errors[x + offset][y + 1].b += errorB * 5.0 / 16.0;
                errors[x + offset - 1][y + 1].b += errorB * 3.0 / 16.0;

                values[j] = findNeighbour({R, G, B}, threeBitDedicatedPalette);
            }
            else
                values[j] = findNeighbour(color, threeBitDedicatedPalette);

            j++;
        }
    }

    char* compressed = compressFrom8to3Bytes(values, (WIDTH / 2) * (HEIGHT / 2));
    vector<char> temp = ByteRunCompression(compressed, (WIDTH / 2) * (HEIGHT / 2)* 3 / 8);

    for(int i = 0; i < temp.size(); i++)
         output.write(&temp[i], sizeof(char));

    output.close();
}

ThreeBytes convertFrom8to3Bytes(Uint8* eightBytes) {
    ThreeBytes threeBytes;

    threeBytes.firstByte = (eightBytes[0] << 5) | (eightBytes[1] << 2) | (eightBytes[2] >> 1);
    threeBytes.secondByte = (eightBytes[2] << 7) | (eightBytes[3] << 4) | (eightBytes[4] << 1) | (eightBytes[5] >> 2);
    threeBytes.thirdByte = (eightBytes[5] << 6) | (eightBytes[6] << 3) | (eightBytes[7]);

    return threeBytes;
}

EightBytes convertFrom3to8Bytes(Uint8* threeBytes) {
    EightBytes eightBytes;

    eightBytes.bytes[0] = (threeBytes[0] >> 5) & (0b00000111);
    eightBytes.bytes[1] = (threeBytes[0] >> 2) & (0b00000111);
    eightBytes.bytes[2] = ((threeBytes[0] << 1) & (0b00000110)) | ((threeBytes[1] >> 7) & (0b00000001));
    eightBytes.bytes[3] = (threeBytes[1] >> 4) & (0b00000111);
    eightBytes.bytes[4] = (threeBytes[1] >> 1) & (0b00000111);
    eightBytes.bytes[5] = ((threeBytes[1] << 2) & (0b00000100)) | ((threeBytes[2] >> 6) & (0b00000011));
    eightBytes.bytes[6] = (threeBytes[2] >> 3) & (0b00000111);
    eightBytes.bytes[7] = (threeBytes[2]) & (0b00000111);

    return eightBytes;
}

char* compressFrom8to3Bytes(Uint8* dataToCompress, int length) {
    char* compressedData = new char[length * 3 / 8];
    Uint8* eightBytes = new Uint8[8];
    ThreeBytes bytesTmp;
    int numberOfBytes = 0;

    for(int i = 0; i < length; i += 8) {
        for(int j = 0; j < 8; j++)
            eightBytes[j] = dataToCompress[i+j];

        bytesTmp = convertFrom8to3Bytes(eightBytes);
        compressedData[numberOfBytes] = bytesTmp.firstByte;
        compressedData[numberOfBytes + 1] = bytesTmp.secondByte;
        compressedData[numberOfBytes + 2] = bytesTmp.thirdByte;
        numberOfBytes += 3;
    }

    return compressedData;
}

template <typename T>
vector<char> decompressFrom3To8Bytes(int length, T dataToDecompress){

    vector<char> decompressedData;
    Uint8* threeBytes = new Uint8[3];
    Uint8* eightBytes = new Uint8[8];
    EightBytes bytesTmp;
    int numberOfBytes = 0;

    for(int i = 0; i < length; i += 3){
        for(int j = 0; j < 3; j++)
            threeBytes[j] = dataToDecompress[i + j];

        bytesTmp = convertFrom3to8Bytes(threeBytes);
        eightBytes = bytesTmp.bytes;

        for(int j = 0; j < 8; j++)
            decompressedData.push_back(eightBytes[j]);
        numberOfBytes += 8;
    }

    return decompressedData;
}

void clear_dedicatedPaletteColorCount() {
    dedicatedPaletteColorCount = 0;
}

void increment_dedicatedPaletteColorCount() {
    dedicatedPaletteColorCount++;
}

int get_dedicatedPaletteColorCount() {
    return dedicatedPaletteColorCount;
}
