#include <iostream>
#include <string>
#include <ctime>
#include <openssl/sha.h>
#include <chrono>
#include <thread> 
#include <vector>
#include "db.h"
#include "multithread.h"
using namespace std;

int adjustDifficulty(struct Block* prevBlock, struct Block* currentBlock, int currentDifficulty, int target);


struct Block {
    Block* next;
    Block* prev;
    string prevHash; 
    string Hash;
    string data;
    int nonce;
    time_t transactions;

    Block(string prevhash, string Data, int difficulty) {
        this->prevHash = prevhash;
        string returnHash;
        
        Mining mineIt(4, difficulty);
        this->nonce = mineIt.mineBlock(prevHash, Data, returnHash);
        this->data = Data;
        this->transactions = time(0);  
        next = nullptr;
        prev = nullptr;
        this->Hash = returnHash;

        
        
    }

};



class BlockChain {
private:
    Block* head;
    Block* tail;
    
public: 

    int difficulty = 7; // number of leading zeros required in hash 
    int target = 60; 
    
    BlockChain() {
        head = nullptr;
        tail = nullptr;
    }

    Block* getTail(){
        return tail;
    }

    
    void AppendBlock(string data) {
        string previousHash = (tail == nullptr) ? "0" : tail->Hash; 

        Block* newBlock = new Block(previousHash, data, difficulty);
        if (head == nullptr) {
            head = newBlock;
            tail = newBlock;
        } else {
            tail->next = newBlock;
            newBlock->prev = tail;
            tail = newBlock;
        }

        
       if (newBlock->prev != nullptr){
        difficulty = adjustDifficulty(newBlock->prev, newBlock, difficulty, target);
       }


        

        cout << "Successfully inserted block with hash: " << newBlock->Hash << endl;
    }

    
    void printChain() {
        Block* current = head;
        unsigned int i = 0;

        while (current != nullptr) {
            cout << "Block #" << i << " contains the following specifications: " << endl;
            cout << "Previous Hash: " << current->prevHash << endl;
            cout << "Current Hash: " << current->Hash << endl;
            cout << "Nonce: " << current->nonce << endl;
            cout << "Transactions: " << current->transactions << endl;
            cout << "\n" << endl;
            current = current->next;
            i++;
        }
    }

    // Retrieve a block by its hash
    Block* RetrieveBlock(string inputHash) {
        Block* current = head;
        while (current != nullptr) {
            if (current->Hash == inputHash) {
                return current;
            }
            current = current->next;
        }
        return nullptr; // If block not found
    }


    bool validateChain() {
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

    // Delete a block by its hash
    bool deleteBlock(string targetHash) {
        Block* current = head;

        // Find the block to delete
        while (current != nullptr && current->Hash != targetHash) {
            current = current->next;
        }

        if (current == nullptr) {
            return false; // Block not found
        }

        // If the block is the head
        if (current == head) {
            head = head->next;
            if (head != nullptr) {
                head->prev = nullptr;
            }
        } else if (current == tail) { // If the block is the tail
            tail = tail->prev;
            if (tail != nullptr) {
                tail->next = nullptr;
            }
        } else { // Middle block
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }

        delete current;
        return true;
    }
};


int adjustDifficulty(Block* prevBlock, Block* currentBlock, int currentDifficulty, int target){
    time_t currentTime = currentBlock->transactions - prevBlock->transactions; // find time diference
    double calculateTarget = currentTime; // calculates current time but in minutes 
    int total = static_cast<int>(calculateTarget - target);
    
    // Base Case

    if (total >= -60 && total <= 60){
        return currentDifficulty; 
    }

    if (total > 60){
        return adjustDifficulty(prevBlock, currentBlock, currentDifficulty - 1, target);

    }
    
    if (total < -60){
        return adjustDifficulty(prevBlock, currentBlock, currentDifficulty + 1, target);
    }


    

    return currentDifficulty; 



    


}   

int main() {

    

    BlockChain blockchain;
    
    
    cout << "Mining block 1... " << endl;
    cout << "\n";
    auto start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Candidate X");
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Block 1 took: " << elapsed.count() << " seconds to mine " <<endl; 
    cout << "\n";

    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB("phillip", "Boll", "Candidate X");
    cout << "Mining block 2... " << endl;
    cout << "\n";

    start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Candidate Y");
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Block 2 took: " << elapsed.count() << " seconds to mine " <<endl; 
    cout << "\n";

    cout << "Previous Difficulty (leading zeros): " << blockchain.difficulty << endl;
    blockchain.difficulty = adjustDifficulty(blockchain.getTail()->prev, blockchain.getTail(), blockchain.difficulty, 5);
    cout << "Updated Difficulty (after calculation of need): " << blockchain.difficulty << endl;
    cout << "\n";

    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB("Karina", "Santos", "Candidate Y");
    cout << "Mining block 3... " << endl;
    cout << "\n";

    start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Barack Obama");
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "\n";
    cout << "Block 3 took: " << elapsed.count() << " seconds to mine " <<endl; 

    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB("Adrian", "Kolber", "Candidate Z");
    cout << "\n";
    cout << "Is blockchain valid? " << (blockchain.validateChain() ? "Yes" : "No") << endl;



    


   //eraseDB();
    /*string hash = "null";
   if (blocksPresent() == true){
    hash = getLatestHash();

   }

   */

   

    return 0; 
}