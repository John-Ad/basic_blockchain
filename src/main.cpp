#include "Blockchain.h"

int main(){
    /*  debugging
    SHA256 sha256;
    string in="hello world";

    cout<<sha256.getHash((unsigned char*)in.c_str(),in.length());
    */

    cout<<endl<<"mining genesis block: ..."<<endl;
    BlockChain chain(1);

    cout<<endl<<"mining next block: "<<endl;
    chain.addBlock("JohnAd","its kinda working");
}




