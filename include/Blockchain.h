#include "Block.h"

class BlockChain{
    private:
        vector<Block> chain;
        uint32 difficulty;
    public:
        BlockChain(int difficulty);
        Block& getLastBlock();
        Block& getBlock(int index);
        void addBlock(string userName, string comment);
};

