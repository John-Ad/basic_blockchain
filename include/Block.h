#include "SHA256.h"

struct Data{            // example of stored data
    string userName;    
    string comment;     // comment left by user
    time_t now;         // time of data was placed in a block (num of seconds since 1970...)

    // constructors
    Data(){
        userName="";
        comment="";
        now=time(0);
    }

    Data(string userName,string comment){   
        this->userName=userName;
        this->comment=comment;
        now=time(0);    // set now to current time
    }
    // copy constructors
    Data(const Data& old){
        this->userName=old.userName;
        this->comment=old.comment;
        this->now=old.now;
    }

    // returns all data concatenated together
    string toString(){  
        return userName+comment+to_string(now);
    }
};

class Block{
    private:
        uint32 index;       // position in chain
        int64_t nonce;      // single use random number that will be added to the constant data used in the hash. The nonce will be changed until the hash produced has a certain number of zeros at the beginning (difficulty)
        Data data;
        string blockHash;   // hash of this block

        void mine(const uint32 difficulty);
        string calculateHash();
    public:
        // basic constructor
        Block(uint32 index, Data data, string prevHash);
        // copy constructor
        Block(const Block& old);

        void mineBlock(const uint32 difficulty, int numOfThreads);

        string prevHash;   // previous block

        // getters/setters
        string getHash();
        Data getData();
        uint32 getIndex();
        int64_t getNonce();
};
