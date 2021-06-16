#include "Blockchain.h"

int main(){
    /*  debugging
    SHA256 sha256;
    string in="hello world";

    cout<<sha256.getHash((unsigned char*)in.c_str(),in.length());
    */

    cout<<endl<<"mining genesis block: ..."<<endl;
    BlockChain chain(4,100);

    cout<<endl<<"mining next block: "<<endl;
    chain.addBlock("JohnAd","its kinda working");

    try{
        cout<<chain.getLastBlock().getData().userName<<endl;
    }catch(const char* err){
        cout<<err<<endl;
    }

    return 0;
}




