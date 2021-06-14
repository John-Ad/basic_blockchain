#include "Block.h"

// basic constructor
Block::Block(uint32 index, Data data, string prevHash): index{index}, data{data}, prevHash{prevHash}, nonce{0}  // nonce starts at 0
{}

// copy constructor
Block::Block(const Block& old){
    this->index=old.index;
    this->nonce=old.nonce;
    this->data=old.data;
    this->blockHash=old.blockHash;
}

string Block::getHash(){
    return blockHash;
}
void Block::mineBlock(const uint32 difficulty){
    // create array of zeros
    //      remove this and just use a string and append zeros
    char zeroPadding[difficulty+1];     // this will be compared to the zeros in the beginning of the calculated hash. If it matches the mining will be complete
    for(int i=0;i<difficulty;i++){
        zeroPadding[i]='0';
    }
    zeroPadding[difficulty]='\0';   // add termination character

    string str(zeroPadding);        // convert to string

    do{
        blockHash=calculateHash();
        //nonce++;      // start from 0 and increase nonce by 1
        nonce=rand();   // set nonce to a random value
    }while(blockHash.substr(0,difficulty)!=str);    // if hash does not start with right number of zeros

    cout<<"Block mined: "<<blockHash<<endl;
}

string Block::calculateHash(){
    SHA256 sha256;      // used to calculate hash
    stringstream ss;    // produce final string to be hashed

    // concatenate all block data together
    ss<<index;              
    ss<<data.toString();
    ss<<prevHash;
    ss<<nonce;

    //cout<<"string to be hashed: "<<ss.str()<<endl;    // debugging

    //return hash
    return sha256.getHash((unsigned char*)ss.str().c_str(),ss.str().length());
}

