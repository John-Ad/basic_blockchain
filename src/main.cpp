#include "include.h"
#include "SHA256.h"

unsigned int rightRotate(unsigned int elem, int len);
unsigned char* convertToChar(unsigned int elem);

int main(){
    string in="";

    SHA256 sha;
    cout<<sha.getHash((unsigned char*)in.c_str(),in.length())<<endl<<endl;
}
