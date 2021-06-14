#ifndef SHA256_H
#define SHA256_H

#include "include.h"
#include <sstream>

typedef unsigned int uint32;

const uint32 hash_values[8]={
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
};

const uint32 round_constants[64]={
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

class SHA256{
    private:
        //instance variables
        int inputMsgSize;
        int numOfChunks;
        uint32* inputMsg;
        uint32* wBlock;
        uint32* hVals;
        stringstream digest;        // stringstream has no copy constructor. This might cause issues later on. Add cpy constructor to SHA256 or replace stringstream

        // worker methods
        unsigned long pow32();      // return 2^32
        int countbits(unsigned long long number);   // count the number of bits in the binary representation of a number eg 10 = 4 bits
        uint32 rightRotate(uint32 elem, int len);   // rotate bits in elem by a certain len
        unsigned char* convertToChar(uint32 elem);         // convert 32 bit elem to array of 8 bit elems

        // algorithm functions
        void createInputMsg(unsigned char* in, int inN);
        void createWBlock();
        void transform();
        void compress();
        void finalConcat();
    public:
        SHA256(){inputMsgSize=0;numOfChunks=0;hVals=new uint32[8];} //hvals deallocd in destructor
        ~SHA256();
        string getHash(unsigned char* msgIn, int n);
};

#endif
