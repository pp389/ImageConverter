#include "ByteRun.h"

std::vector<char> ByteRunCompression(char input[], int length) {
    int i = 0;
    std::vector<char> compressed;
    //while not all of bytes are compressed
    while(i < length) {
        //sequence of repeated minimum 2 bytes
           if((i < length - 1) && (input[i] == input[i+1])) {
               //measure the length of the sequence
               int counter = 0;
               while((i + counter < length - 1) && (input[i + counter] == input[i + counter + 1]) && counter < 127)
                   counter++;

               compressed.push_back(-counter);
               compressed.push_back(input[i + counter]);

               //move the index by the length of the sequence
               i += (counter + 1);
           }
           //sequence of different bytes
           else {
             //measure the length of the sequence
             int counter = 0;
             while((i + counter < length - 1) &&(input[i + counter] != input[i + counter + 1])&& counter < 127)
                counter++;

             //add the ending
             if((i + counter == length - 1)&& (counter < 127))
                counter++;

             compressed.push_back(counter-1);
             for(int k =0 ; k<counter; k++)
                 compressed.push_back(input[i+k]);
             //move the index by the length of the sequence
             i += counter;
           }

    }

    return compressed;
}


std::vector<char> ByteRunDecompression(char input[], int length){
    int i = 0;
    std::vector<char> decompressed;
    //while not all of bytes are decompressed
    while(i < length)
    {
        //blank code
        if(input[i] == -128)
            i++;
        //sequence of repeating bytes
        else if(input[i] < 0) {
            int counter = -(input[i] - 1);

            for(int k = 0; k < counter; k++)
                decompressed.push_back(input[i + 1]);

            //move the index by the length of the sequence
            i += 2;
        }
        else{
            int counter = input[i] + 1;

            for(int k = 0; k < counter; k++)
                decompressed.push_back(input[i + 1 + k]);

            //move the index by the length of the sequence
            i += input[i]+2;
        }
    }

    return decompressed;
}
