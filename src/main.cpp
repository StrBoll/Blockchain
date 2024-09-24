#include <iostream>
#include <string>
#include <ctime>
#include <openssl/sha.h>
#include <chrono>
#include <thread> 
#include <vector>
#include "db.h"

using namespace std;


int mineBlock(string prevHash, string data, int difficulty, string& returnHash);
int adjustDifficulty(struct Block* prevBlock, struct Block* currentBlock, int currentDifficulty, int target);
string compute256(const string& input);


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
        
        this->nonce = mineBlock(prevHash, Data, difficulty, returnHash);
        this->data = Data;
        this->transactions = time(0);  // the current time at which the block is created 
        next = nullptr;
        prev = nullptr;
        this->Hash = returnHash;

        
        
    }

};

bool validHash(const string& hash, int difficulty){
    int i = 0;
    for (auto character : hash){
        if (character == '0'){
            i++;
        }
        else {
            break;
        }

    }

    return i == difficulty;
}


int mineBlock(string prevHash, string data, int difficulty, string& returnHash){
    int nonce = 0; 
    string temporaryHash; 

    do {
        string input = prevHash + data + to_string(nonce);
        temporaryHash = compute256(input);
        nonce++;
    } while (!validHash(temporaryHash, difficulty));

    returnHash = temporaryHash;

    cout << "Valid hash found at: " << temporaryHash << endl;


    return nonce -1;
}

string HexIt(const unsigned char* input, size_t length) {
    const char* hexAlphabet = "0123456789abcdef";
    string output;
    
    for (size_t i = 0; i < length; i++) {
        output.push_back(hexAlphabet[(input[i] >> 4) & 0xF]);
        output.push_back(hexAlphabet[input[i] & 0xF]);       
    }
    
    return output;
}


string compute256(const string& input){

// Deprecated, newer EVP API is available but since this is just a project I'm doing for learning about blockchain I figured it didn't matter
   unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(hash, &sha256);

    

    return HexIt(hash, SHA256_DIGEST_LENGTH);
}



class BlockChain {
private:
    Block* head;
    Block* tail;
    
public: 

    int difficulty = 5; // number of leading zeros required in hash 
    int target = 60; 
    
    BlockChain() {
        head = nullptr;
        tail = nullptr;
    }

    Block* getTail(){
        return tail;
    }

    
    void AppendBlock(string data) {
        string previousHash = (tail == nullptr) ? "0" : tail->Hash; // if tail is empty then 0, else tail's hash value

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

    // Print all blocks in the blockchain
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

    /*

    BlockChain blockchain;
    
    
    cout << "Mining block 1... " << endl;
    cout << "\n";
    auto start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Donald Trump");
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Block 1 took: " << elapsed.count() << " seconds to mine " <<endl; 
    cout << "\n";

    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);

    cout << "Mining block 2... " << endl;
    cout << "\n";

    start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Kamala Harris");
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Block 2 took: " << elapsed.count() << " seconds to mine " <<endl; 
    cout << "\n";

    cout << "Previous Difficulty (leading zeros): " << blockchain.difficulty << endl;
    blockchain.difficulty = adjustDifficulty(blockchain.getTail()->prev, blockchain.getTail(), blockchain.difficulty, 5);
    cout << "Updated Difficulty (after calculation of need): " << blockchain.difficulty << endl;
    cout << "\n";

    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);

    cout << "Mining block 3... " << endl;
    cout << "\n";

    start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Barack Obama");
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "\n";
    cout << "Block 3 took: " << elapsed.count() << " seconds to mine " <<endl; 

    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    
    cout << "\n";
    cout << "Is blockchain valid? " << (blockchain.validateChain() ? "Yes" : "No") << endl;



    */


   //eraseDB();

   if (blocksPresent() == true){
    string hash = getLatestHash();
    cout << hash << endl;
   }

   

    return 0; 
}