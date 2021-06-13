#include "include.h"
#include "SHA256.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ios>
#include <string>

unsigned int rightRotate(unsigned int elem, int len);
unsigned char* convertToChar(unsigned int elem);

int main(){
    unsigned char aa=20;
    bitset<8> x(aa);

    // original value
    cout<<x<<endl;

    //rightshift by 3
    x=aa>>3;
    cout<<x<<endl;

    //leftshift by 3
    x=aa<<3;
    cout<<x<<endl;

    //or by 45 (check if one of or both bits are 1)
    x=aa|45;
    cout<<x<<endl;

    //xor by 45 (check if both bits are different)
    x=aa^45;
    cout<<x<<endl;

    //aand by 45
    x=aa&45;
    cout<<x<<endl;

    //not (invert all bits)
    x=~aa;
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

    // compression stage
    unsigned int a=hash_values[0],b=hash_values[1],c=hash_values[2],d=hash_values[3],e=hash_values[4],f=hash_values[5],g=hash_values[6],h=hash_values[7];

    unsigned int ch;
    unsigned int maj;
    unsigned int temp1;
    unsigned int temp2;

    for(int i=0;i<64;i++){
        sTran1= rightRotate(e,6) ^ rightRotate(e,11) ^ rightRotate(e,25);
        ch= (e & f) ^ ((~e) & g);
        temp1= (h + sTran1 + ch + round_constants[i] + wBlock[i]) % (unsigned long)pow(2,32);   // all addition mod 2^32
        sTran0= rightRotate(a,2) ^ rightRotate(a,13) ^ rightRotate(a,22);
        maj= (a & b) ^ (a & c) ^ (b & c);
        temp2= (sTran0 + maj) % (unsigned long)pow(2,32);

        h=g;
        g=f;
        f=e;
        e= (d+temp1) % (unsigned long)pow(2,32);
        d=c;
        c=b;
        b=a;
        a= (temp1+temp2) % (unsigned long)pow(2,32);
    }

    // show transformation values
    cout<<"a...h values"<<endl;
    bitset<32> aTohVals[8];
    aTohVals[0]=a;
    aTohVals[1]=b;
    aTohVals[2]=c;
    aTohVals[3]=d;
    aTohVals[4]=e;
    aTohVals[5]=f;
    aTohVals[6]=g;
    aTohVals[7]=h;
    cout<<aTohVals[0]<<endl<<aTohVals[1]<<endl<<aTohVals[2]<<endl<<aTohVals[3]<<endl<<aTohVals[4]<<endl<<aTohVals[5]<<endl<<aTohVals[6]<<endl<<aTohVals[7]<<endl;

    cout<<endl<<endl;

    // set final values
    a= (hash_values[0]+a) % (unsigned long)pow(2,32);   // all addition mod 2^32
    b= (hash_values[1]+b) % (unsigned long)pow(2,32);
    c= (hash_values[2]+c) % (unsigned long)pow(2,32);
    d= (hash_values[3]+d) % (unsigned long)pow(2,32);
    e= (hash_values[4]+e) % (unsigned long)pow(2,32);
    f= (hash_values[5]+f) % (unsigned long)pow(2,32);
    g= (hash_values[6]+g) % (unsigned long)pow(2,32);
    h= (hash_values[7]+h) % (unsigned long)pow(2,32);

    // show final transformation values
    cout<<"a...h final values"<<endl;
    aTohVals[0]=a;
    aTohVals[1]=b;
    aTohVals[2]=c;
    aTohVals[3]=d;
    aTohVals[4]=e;
    aTohVals[5]=f;
    aTohVals[6]=g;
    aTohVals[7]=h;
    cout<<aTohVals[0]<<endl<<aTohVals[1]<<endl<<aTohVals[2]<<endl<<aTohVals[3]<<endl<<aTohVals[4]<<endl<<aTohVals[5]<<endl<<aTohVals[6]<<endl<<aTohVals[7]<<endl;

    cout<<endl<<endl;

    // concat final hash
    ostringstream digest;

    unsigned char* convertionHolder=new unsigned char[4];

    convertionHolder= convertToChar(a);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(b);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(c);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(d);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(e);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(f);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(g);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    convertionHolder= convertToChar(h);
    digest<<hex<<setfill('0')<<setw(2)<<static_cast<int>(convertionHolder[0])<<static_cast<int>(convertionHolder[1])<<static_cast<int>(convertionHolder[2])<<static_cast<int>(convertionHolder[3]);

    cout<<digest.str()<<endl;

    /*
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(b);
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(c);
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(d);
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(e);
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(f);
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(g);
    printf("%x%x%x%x",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);

    convertionHolder= convertToChar(h);
    printf("%x%x%x%x\n",convertionHolder[0],convertionHolder[1],convertionHolder[2],convertionHolder[3]);
    */
}

unsigned int rightRotate(unsigned int elem, int len){   // 32 bit based
    return (elem >> len) | (elem << (32-len));
}

unsigned char* convertToChar(unsigned int elem){

    unsigned char* result=new unsigned char[4];
    result[0]= elem;          // get leftmost bits by shifting to right
    result[1]= (elem<<8) >> 24;     // shift to left once and get leftmost bits by shifting to right
    result[2]= (elem<<16) >> 24;    // shift left twice ...
    result[3]= (elem<<24) >> 24;    // shift left thrice ...
    return result;
}




