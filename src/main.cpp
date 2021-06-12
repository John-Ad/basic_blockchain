#include "include.h"
#include "SHA256.h"
#include <algorithm>
#include <cmath>

unsigned int rightRotate(unsigned int elem, int len);

int main(){
    unsigned char a=20;
    bitset<8> x(a);

    // original value
    cout<<x<<endl;

    //rightshift by 3
    x=a>>3;
    cout<<x<<endl;

    //leftshift by 3
    x=a<<3;
    cout<<x<<endl;

    //or by 45 (check if one of or both bits are 1)
    x=a|45;
    cout<<x<<endl;

    //xor by 45 (check if both bits are different)
    x=a^45;
    cout<<x<<endl;

    //and by 45
    x=a&45;
    cout<<x<<endl;

    //not (invert all bits)
    x=~a;
    cout<<x<<endl;


    cout<<endl<<endl;
    string input="hello world";
    bitset<8> arr[64];
    unsigned char* cstr=(unsigned char*)input.c_str();

    //calculate size of input message
    int n=512;
    while(true){
        if(input.length()>=n-64)
            n+=512;
        else
            break;
    }
    unsigned int* input_message=new unsigned int[n/8];      // 512 is the number of bits, each element stores only 8 bits (1 byte)

    // copy message, add '1' and pad with 0's
    for(int i=0;i<(n/8)-8;i++){     // full number of bits -64 bits. 64/8 = 8 bytes
        if(i<input.length()){
            input_message[i]=cstr[i];
        }else if(i==input.length()){
            input_message[i]=128;           // 128 = 10000000 // this adds the extra 1 needed
        }else{
            input_message[i]=0;
        }
    }

    // display padded message binary
    cout<<"message minus 64 bits"<<endl;
    bitset<8> binary;
    for(int i=0;i<(n/8)-8;i++){
        if(i%8==0 && i!=0)
            cout<<endl;

        binary=input_message[i];
        cout<<binary<<", ";
    }
    cout<<endl<<endl;

    // calculate and store length of input(number of bytes)
    unsigned long long inLen=8*input.length();     // 8 bits for each character

    for(int i=(n/8)-1;i>=(n/8)-8;i--){  // start from end of array and count back 8 bytes
        if(inLen==0){
            input_message[i]=0;
        }else if(inLen%255==0){
            input_message[i]=255;
            inLen-=255;
        }else if(inLen%255!=0){
            input_message[i]=inLen;
            inLen-=inLen;
        }
    }

    // display msg binary with added 64 bits
    cout<<"input + 64 bits"<<endl;
    for(int i=0;i<(n/8);i++){
        if(i%8==0 && i!=0)
            cout<<endl;

        binary=input_message[i];
        cout<<binary<<", ";
    }

    cout<<endl<<endl;

    // create new array where each entry is 32 bits long
    unsigned int* wBlock=new unsigned int[64];
    fill(&wBlock[0],&wBlock[63],0); // fill array with 0

    int wIndex=0;
    for(int i=0;i<(n/8);i++){   // loop through input_message array
        if(i!=0 && i%4==0)
            wIndex++;

        wBlock[wIndex]= wBlock[wIndex] << 8 | input_message[i];     // shift left by 8 bits and perform OR using input section
    }

    // show wblock
    bitset<32> binary32;
    cout<<"wblock"<<endl;
    for(int i=0;i<64;i++){
        if(i%8==0 && i!=0)
            cout<<endl;

        binary32=wBlock[i];
        cout<<binary32<<", ";
    }

    cout<<endl<<endl;


    // perform transformation algorithm
    unsigned int sTran0;
    unsigned int sTran1;
    for(int i=16;i<64;i++){
        sTran0= rightRotate(wBlock[i-15],7) ^ rightRotate(wBlock[i-15],18) ^ (wBlock[i-15] >> 3);       // step 1
        sTran1= rightRotate(wBlock[i-2],17) ^ rightRotate(wBlock[i-2],19) ^ (wBlock[i-2] >> 10);        // step 2
        wBlock[i]= (wBlock[i-16] + sTran0 + wBlock[i-7] + sTran1) % (unsigned long)pow(2,32);     // add the values mod 32
    }

    // show wblock
    cout<<"wblock"<<endl;
    for(int i=0;i<64;i++){
        if(i%8==0 && i!=0)
            cout<<endl;

        binary32=wBlock[i];
        cout<<binary32<<", ";
    }

    cout<<endl<<endl;
}

unsigned int rightRotate(unsigned int elem, int len){   // 32 bit based
    return (elem >> len) | (elem << (32-len));
}




