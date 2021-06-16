#include "Block.h"

// basic constructor
Block::Block(uint32 index, Data data, string prevHash): blockHash{""}, index{index}, data{data}, prevHash{prevHash}, nonce{0}  // nonce starts at 0
{}

// copy constructor
Block::Block(const Block& old){
    this->index=old.index;
    this->nonce=old.nonce;
    this->data=old.data;
    this->blockHash=old.blockHash;
}

void Block::mine(const uint32 difficulty){
    string hashHolder;      // local holder for hash to be used by thread

    // create array of zeros
    string str="";
    for(int i=0;i<difficulty;i++){
        str+="0";
    }

    do{
        if(blockHash != "")       // if hash is found by another thread stop current threads execution
            return;

        hashHolder=calculateHash();
        nonce=rand();   // set nonce to a random value
    }while(hashHolder.substr(0,difficulty)!=str);    // if hash does not start with right number of zeros

    blockHash=hashHolder;       // set blocks final hash
    cout<<"Block mined: "<<blockHash<<endl;
}

void Block::mineBlock(const uint32 difficulty, int numOfThreads){
    vector<thread> threads;     // used to store all the created threads

    for(int i=0;i<numOfThreads;i++){    // create num of threads as defined by user
        // pass pointer-to-member, object, params
        threads.push_back(thread(&Block::mine,this,difficulty)); // push thread to arr of threads
    }

    for(int i=0;i<threads.size();i++){  // join all threads
        threads[i].join();
    }
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

// getters/setters
string Block::getHash(){
    return blockHash;
}
Data Block::getData(){
    return this->data;
}
uint32 Block::getIndex(){
    return this->index;
}
int64_t Block::getNonce(){
    return this->nonce;
}

