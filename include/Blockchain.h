#include "Block.h"

class BlockChain{
    private:
        vector<Block> chain;
        uint32 difficulty;
        int numOfThreads;
    public:
        BlockChain(uint32 difficulty, int numOfThreads);
        Block& getLastBlock();
        Block& getBlock(int index);
        void addBlock(string userName, string comment);
};

