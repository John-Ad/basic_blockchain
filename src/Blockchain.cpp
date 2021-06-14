#include "Blockchain.h"

BlockChain::BlockChain(int difficulty){
    srand(time(0));                     // set seed for random generator
    this->difficulty=difficulty;           // default difficulty is 1
    addBlock("GENESIS BLOCK","GENESIS BLOCK");  // add genesis block as first block in chain
}

Block& BlockChain::getLastBlock(){
    return chain[chain.size()-1];  // return reference to last block
}
Block& BlockChain::getBlock(int index){
    return chain[index];
}

void BlockChain::addBlock(string userName,string comment){
    // get previous hash or set to nothing if the block being added is the first
    string prevHash = (chain.size()>0)? getLastBlock().getHash() : "";
    cout<<endl<<"previous blocks hash: "<<prevHash<<endl<<endl;

    Data data(userName, comment);   // construct data object

    Block block(chain.size(),data,prevHash); // create block
    block.mineBlock(difficulty);    // mine block

    chain.push_back(block);  // once mining os done add to chain
}
