#include "Blockchain.h"

BlockChain::BlockChain(uint32 difficulty, int numOfThreads){
    this->difficulty=difficulty;           // default difficulty is 1
    this->numOfThreads=numOfThreads;
    addBlock("GENESIS BLOCK","GENESIS BLOCK");  // add genesis block as first block in chain
}

Block& BlockChain::getLastBlock(){
    if(chain.size()>0)
        return chain[chain.size()-1];
    throw "\ngetLastBlock() error: chain is empty"; 
}
Block& BlockChain::getBlock(int index){     // throws index out of bounds exception
    if(index>=0 && index<chain.size())
        return chain[index];
    throw "\ngetBlock(index) error: index out of bounds"; 
}

void BlockChain::addBlock(string userName,string comment){
    // get previous hash or set to nothing if the block being added is the first
    string prevHash = (chain.size()>0)? getLastBlock().getHash() : "";
    cout<<endl<<"previous blocks hash: "<<prevHash<<endl<<endl;

    Data data(userName, comment);   // construct data object

    Block block(chain.size(),data,prevHash); // create block
    block.mineBlock(difficulty,numOfThreads);    // mine block

    chain.push_back(block);  // once mining os done add to chain
}
