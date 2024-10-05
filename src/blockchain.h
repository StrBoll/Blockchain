#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <string>
#include <ctime>

int adjustDifficulty(struct Block* prevBlock, struct Block* currentBlock, int currentDifficulty, int target);

struct Block {
    Block* next;
    Block* prev;
    std::string prevHash; 
    std::string Hash;
    std::string data;
    int nonce;
    time_t transactions;

    Block(std::string prevhash, std::string Data, int difficulty, std::string& miningResult);
    Block(std::string prevhash, std::string Data, bool SHA);
};

class BlockChain {
private:
    Block* head;
    Block* tail;

public: 
    int difficulty; // number of leading zeros required in hash 
    int target; 
    
    BlockChain(); // Constructor
    Block* getHead();
    Block* getTail(); // Get the last block in the chain
    void AppendBlock(std::string data, std::string& miningResult); // Add a block to the blockchain
    void printChain(); // Print all blocks in the blockchain
    bool clearChain(); // clear blockchain
    Block* RetrieveBlock(std::string inputHash); // Retrieve a block by its hash
    bool validateChain(); // Validate the blockchain
    bool deleteBlock(std::string targetHash); // Delete a block by its hash
    void setHead(Block* head);
    void setTail(Block* tail);
};

bool databaseToChain(BlockChain &blockchain);

#endif
