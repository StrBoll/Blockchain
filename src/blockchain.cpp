#include "blockchain.h"
#include <iostream>
#include <string>
#include <ctime>
#include "multithread.h"  
#include <pqxx/pqxx>


using namespace pqxx;

Block::Block(std::string prevhash, std::string Data, int difficulty, std::string& miningResult, int& totalHashTime) {
    this->prevHash = prevhash;
    std::string returnHash;
    
    Mining mineIt(8, difficulty);
    this->nonce = mineIt.mineBlock(prevHash, Data, returnHash, miningResult, totalHashTime);
    this->data = Data;
    this->transactions = time(0);  
    next = nullptr;
    prev = nullptr;
    this->Hash = returnHash;
}

Block::Block(std::string prevhash, std::string Data, bool SHA){
    this->prevHash = prevhash;
    this->data = Data;
    this->transactions = time(0);
    next = nullptr;
    prev = nullptr;

}


BlockChain::BlockChain() {
    head = nullptr;
    tail = nullptr;
    difficulty = 6; 
    target = 60;
}

Block* BlockChain::getTail() {
    return tail;
}

Block* BlockChain::getHead(){
    return head;
}

void BlockChain::setTail(Block* tail){
    this->tail = tail;
}

void BlockChain::setHead(Block* head){
    this->head = head;
}

void BlockChain::AppendBlock(std::string data, std::string& miningResult, int& totalHashTime) {
    std::string previousHash = (tail == nullptr) ? "0" : tail->Hash; 

    Block* newBlock = new Block(previousHash, data, difficulty, miningResult, totalHashTime);
    if (head == nullptr) {
        head = newBlock;
        tail = newBlock;
    } else {
        tail->next = newBlock;
        newBlock->prev = tail;
        tail = newBlock;
    }

    if (newBlock->prev != nullptr) {
        difficulty = adjustDifficulty(newBlock->prev, newBlock, difficulty, target);
    }

    std::cout << "Successfully inserted block with hash: " << newBlock->Hash << std::endl;
}

bool BlockChain::clearChain(){

    Block* current = head;
    while (current != nullptr){
        Block* temp = current;
        current = current->next;
        delete temp;
    }

    head = nullptr;
    tail = nullptr;

    if (current== nullptr){
        return true;
    }

    return false;


}

void BlockChain::printChain() {
    Block* current = head;
    unsigned int i = 0;

    while (current != nullptr) {
        std::cout << "Block #" << i << " contains the following specifications: " << std::endl;
        std::cout << "Previous Hash: " << current->prevHash << std::endl;
        std::cout << "Current Hash: " << current->Hash << std::endl;
        std::cout << "Nonce: " << current->nonce << std::endl;
        //std::cout << "Transactions: " << current->transactions << std::endl;
        std::cout << "\n" << std::endl;
        current = current->next;
        i++;
    }
}


Block* BlockChain::RetrieveBlock(std::string inputHash) {
    Block* current = head;
    while (current != nullptr) {
        if (current->Hash == inputHash) {
            return current;
        }
        current = current->next;
    }
    return nullptr; 
}


bool BlockChain::validateChain() {
    Block* current = head;

    if (current == nullptr || current->next == nullptr) {
        return true; 
    }

    current = current->next; 
    while (current != nullptr) {
        if (current->prevHash != current->prev->Hash) {
            return false;
        }
        current = current->next;
    }

    return true;
}


bool BlockChain::deleteBlock(std::string targetHash) {
    Block* current = head;

    while (current != nullptr && current->Hash != targetHash) {
        current = current->next;
    }

    if (current == nullptr) {
        return false; 
    }

    if (current == head) {  
        head = head->next;
        if (head != nullptr) {
            head->prev = nullptr;
        }
    } else if (current == tail) { 
        tail = tail->prev;
        if (tail != nullptr) {
            tail->next = nullptr;
        }
    } else { 
        current->prev->next = current->next;
        current->next->prev = current->prev;
    }

    delete current;
    return true;
}

int adjustDifficulty(Block* prevBlock, Block* currentBlock, int currentDifficulty, int target) {
    time_t currentTime = currentBlock->transactions - prevBlock->transactions;
    double calculateTarget = currentTime;
    int total = static_cast<int>(calculateTarget - target);

    if (total >= -60 && total <= 60) {
        return currentDifficulty; 
    }

    if (total > 60) {
        return adjustDifficulty(prevBlock, currentBlock, currentDifficulty - 1, target);
    }
    
    if (total < -60) {
        return adjustDifficulty(prevBlock, currentBlock, currentDifficulty + 1, target);
    }

    return currentDifficulty; 
}

bool databaseToChain(BlockChain &blockchain){
    try {
    connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");

    nontransaction N(C);

    string initCommand = "SELECT block_number, block_number, previous_hash, current_hash, nonce, transaction_time FROM blocks ORDER BY block_number; ";

    string secondCommand = "SELECT voterFirst, VoterLast, candidate_vote FROM votes;";

    result query = N.exec(initCommand);


    if (query.empty()){
        
        cerr << "No blocks in database" << endl;
        return false;
    } 

    result query2 = N.exec(secondCommand);

    if (query2.empty()) {

        cerr << "No votes found in database" << endl;
        return false;
    }

    auto iter = query.begin();
    auto iter2 = query2.begin();

    for (auto iter = query.begin(); iter != query.end(); ++iter){

        string prevHash = (*iter)["previous_hash"].is_null() ? "0" : (*iter)["previous_hash"].as<string>();
        string currHash = (*iter)["current_hash"].is_null() ? "" : (*iter)["current_hash"].as<string>();

        int nonce = (*iter)["nonce"].as<int>();
        //time_t transactions = (*iter)["transaction_time"].as<time_t>();
        string voterFirstName = (*iter2)["voterFirst"].as<string>();
        string voterLastName = (*iter2)["VoterLast"].as<string>();
        string candidateVote = (*iter2)["candidate_vote"].as<string>();

        ++iter2;

        // need to implement putting the voter info back into the blocks
        bool noSHA = true;
        Block* newBlock = new Block(prevHash, candidateVote, noSHA);
        newBlock->Hash = currHash;
        newBlock->nonce = nonce;
        //newBlock->transactions = transactions;

        if (blockchain.getTail() == nullptr) {
            newBlock->prevHash = "0";
            blockchain.setHead(newBlock);
            blockchain.setTail(newBlock);
        } else {
            blockchain.getTail()->next = newBlock;
            newBlock->prev = blockchain.getTail();
            newBlock->prevHash = blockchain.getTail()->Hash; 
            blockchain.setTail(newBlock);
        }





        
    }
    return true;

    


}

catch (const exception &e){
    cerr << e.what() << endl;
    return false;
}


}

