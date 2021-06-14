#include "Blockchain.h"

int main(){
    cout<<endl<<"mining genesis block: ..."<<endl;
    BlockChain chain(1);

    cout<<endl<<"mining next block: "<<endl;
    chain.addBlock("JohnAd","its kinda working");
}


