#include "SHA256.h"

// destructor
SHA256::~SHA256(){      // delete heap objects 
    delete[] inputMsg;
    delete[] wBlock;
    delete[] hVals;
}

unsigned long SHA256::pow32(){
    return pow(2,32);
}
int SHA256::countbits(unsigned long long number){
    return (int)log2(number)+1;
}
uint32 SHA256::rightRotate(uint32 elem, int len){   // 32 bit based
    return (elem >> len) | (elem << (32-len));
}
unsigned char* SHA256::convertToChar(unsigned int elem){
    unsigned char* result=new unsigned char[4];     // deallocated in finalConcat
    result[0]= elem >> 24;          // get leftmost bits by shifting to right
    result[1]= (elem<<8) >> 24;     // shift to left once and get leftmost bits by shifting to right
    result[2]= (elem<<16) >> 24;    // shift left twice ...
    result[3]= (elem<<24) >> 24;    // shift left thrice ...
    return result;
}

void SHA256::createInputMsg(unsigned char* in, int n){
    // calculate message size
    int size=512;   // size in bits
    while(true){
        if(n*8>=size-64)
            size+=512;
        else
            break;
    }

    inputMsgSize=size/8;
    numOfChunks=size/512;

    // create array of size size/8      // deallocated in destructor
    inputMsg=new uint32[size/8];     // 8 bits in a byte eg 512 bits / 8 = 64 bytes

    // copy message, add '1' and pad with 0s
    for(int i=0;i<(size/8)-8;i++){     // full number of bits - 64 bits. 64/8 = 8 bytes
        if(i<n){
            inputMsg[i]=in[i];
        }else if(i==n){
            inputMsg[i]=128;           // 128 = 10000000 // this adds the extra '1' needed
        }else{
            inputMsg[i]=0;
        }
    }

    // calculate length of input in bits and convert to binary // this makes up the remaining 64 bits for the output message
    unsigned long long outLen=8*n;  // 8 bits in a byte, each byte represents 1 character
    int lenHolder=countbits(outLen);    

    int bitShift=8;
    for(int i=(size/8)-1;i>=(size/8)-8;i--){    // start from end of array and count back 8 bytes(64 bits)
        if(lenHolder>0){
            inputMsg[i]= (outLen << (outLen - bitShift)) >> (outLen - 8);    // shift the chosen sections to the front, then shift the front section to the back leaving only the seeked 8 bits
            bitShift+=8;    // move to next section of bits from right
            lenHolder-=8;   // 8 bits used each time, decrease by this amount
        }else{
            inputMsg[i]=0;
        }
    }
}


void SHA256::createWBlock(){
    wBlock=new uint32[64];      // deallocated in destructor

    // init hash values
    hVals[0]=hash_values[0];
    hVals[1]=hash_values[1];
    hVals[2]=hash_values[2];
    hVals[3]=hash_values[3];
    hVals[4]=hash_values[4];
    hVals[5]=hash_values[5];
    hVals[6]=hash_values[6];
    hVals[7]=hash_values[7];

    int startIndex=0;
    int wIndex;

    for(int i=0;i<numOfChunks;i++){     // chunk loop //each chunk = 512 bits
        fill(&wBlock[0],&wBlock[63],0); // fill wblock with 0's

        // add inputMsg to first 16 bytes of wblock
        wIndex=0;
        for(int e=startIndex;e<startIndex+(512/8);e++){   // loop through input_message array
            if(e!=startIndex && e%4==0)  // 32 bits in word = 8 * 4 bits
                wIndex++;       // move to next elem when 32 bits written to current one

            wBlock[wIndex]= wBlock[wIndex] << 8 | inputMsg[e];     // shift left by 8 bits and perform OR using input section
        }

        // transform current wblock
        transform();

        // compress wblock and add to final hash_vals
        compress();

        startIndex+=(512/8);    // move start index to next section
    }
}

void SHA256::transform(){
    uint32 sTran0;
    uint32 sTran1;
    for(int i=16;i<64;i++){
        sTran0= rightRotate(wBlock[i-15],7) ^ rightRotate(wBlock[i-15],18) ^ (wBlock[i-15] >> 3);       // step 1
        sTran1= rightRotate(wBlock[i-2],17) ^ rightRotate(wBlock[i-2],19) ^ (wBlock[i-2] >> 10);        // step 2
        wBlock[i]= (wBlock[i-16] + sTran0 + wBlock[i-7] + sTran1) % pow32();     // add the values mod 32
    }
}

void SHA256::compress(){
    // working variables a...h
    uint32 a,b,c,d,e,f,g,h;  
    a=hVals[0];
    b=hVals[1];
    c=hVals[2];
    d=hVals[3];
    e=hVals[4];
    f=hVals[5];
    g=hVals[6];
    h=hVals[7];

    // holder variables
    uint32 sTran0;
    uint32 sTran1;
    uint32 ch;
    uint32 maj;
    uint32 temp1;
    uint32 temp2;

    for(int i=0;i<64;i++){
        sTran1= rightRotate(e,6) ^ rightRotate(e,11) ^ rightRotate(e,25);
        ch= (e & f) ^ ((~e) & g);
        temp1= (h + sTran1 + ch + round_constants[i] + wBlock[i]) % pow32();   // all addition mod 2^32
        sTran0= rightRotate(a,2) ^ rightRotate(a,13) ^ rightRotate(a,22);
        maj= (a & b) ^ (a & c) ^ (b & c);
        temp2= (sTran0 + maj) % pow32();

        h=g;
        g=f;
        f=e;
        e= (d+temp1) % pow32();
        d=c;
        c=b;
        b=a;
        a= (temp1+temp2) % pow32();
    }

    // had a...h to final hash vals
    hVals[0]=(hVals[0]+a)%pow32();
    hVals[1]=(hVals[1]+b)%pow32();
    hVals[2]=(hVals[2]+c)%pow32();
    hVals[3]=(hVals[3]+d)%pow32();
    hVals[4]=(hVals[4]+e)%pow32();
    hVals[5]=(hVals[5]+f)%pow32();
    hVals[6]=(hVals[6]+g)%pow32();
    hVals[7]=(hVals[7]+h)%pow32();
}

void SHA256::finalConcat(){

    for(int i=0;i<8;i++){
        unsigned char* conversionHolder = convertToChar(hVals[i]);
        digest<<hex<<setfill('0')<<setw(2)<<(int)conversionHolder[0];
        digest<<hex<<setfill('0')<<setw(2)<<(int)conversionHolder[1];
        digest<<hex<<setfill('0')<<setw(2)<<(int)conversionHolder[2];
        digest<<hex<<setfill('0')<<setw(2)<<(int)conversionHolder[3];

        // conversionHolder no longer needed, delete from heap
        delete[] conversionHolder;
    }
    //cout<<endl<<endl<<digest.str()<<endl;     // debugging
}

string SHA256::getHash(unsigned char* msgIn, int n){
    createInputMsg(msgIn,n);

    // msgIn has been used, delete from heap
    //delete msgIn;     // no need, somewhere else calls delete, I dont know where  // results in: free(): double free detected in tcache 2 error

    //cout<<endl<<inputMsgSize*8<<endl<<endl;   // debugging

    createWBlock();     // convert input to wblocks for transformation and compression

    finalConcat();

    return digest.str();
}
