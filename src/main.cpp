#include <iostream>
#include <string>
#include <ctime>
#include <openssl/sha.h>
using namespace std;



int adjustDifficulty(Block* prevBlock, Block* currentBlock, int currentDifficulty, int target){
    time_t currentTime = currentBlock->transactions - prevBlock->transactions; // find time diference
    double calculateTarget = currentTime / 60.0; // calculates current time but in minutes 
    int total = static_cast<int>(calculateTarget - target);
    
    // Base Case

    if (total >= -2 && total <= 2){
        return currentDifficulty; 
    }

    if (total > 2){
        return adjustDifficulty(prevBlock, currentBlock, currentDifficulty - 1, target);

    }
    
    if (total < -2){
        return adjustDifficulty(prevBlock, currentBlock, currentDifficulty + 1, target);
    }


    

    return currentDifficulty; 



    


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


struct Block {
    Block* next;
    Block* prev;
    string prevHash; 
    string Hash;
    string data;
    int nonce;
    time_t transactions;

    Block(string prevhash, int Nonce, string Data) {
        this->prevHash = prevhash;
        this->nonce = Nonce;
        this->data = Data;
        this->transactions = time(0);  // the current time at which the block is created 
        next = nullptr;
        prev = nullptr;
        this->Hash = setHash();
        
    }


    string setHash() const {
        string blockData = prevHash + to_string(nonce) + to_string(transactions) + data;
        return compute256(blockData);
    }
};


class BlockChain {
private:
    Block* head;
    Block* tail;

public: 
    
    BlockChain() {
        head = nullptr;
        tail = nullptr;
    }

    
    void AppendBlock(string data, int insertNonce) {
        string previousHash = (tail == nullptr) ? "0" : tail->Hash; // if tail is empty then 0, else tail's hash value
        Block* newBlock = new Block(previousHash, insertNonce, data);
        if (head == nullptr) {
            head = newBlock;
            tail = newBlock;
        } else {
            tail->next = newBlock;
            newBlock->prev = tail;
            tail = newBlock;
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

    // Validate the blockchain (check if each block's prevHash matches the previous block's Hash)
    bool validateChain() {
        Block* current = head;

        if (current == nullptr || current->next == nullptr) {
            return true; // Empty or single block chains are valid
        }

        current = current->next; // Start from the second block
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

int main() {


    

    BlockChain blockchain;

    // Adding some blocks to the blockchain
    blockchain.AppendBlock("Vote: Donald Trump", 1);
    blockchain.AppendBlock("Vote: Kamala Harris", 2);
    blockchain.AppendBlock("Vote: Robert Kennedy Jr.", 3);

    // Print the blockchain
    blockchain.printChain();

    // Validate the chain
    if (blockchain.validateChain()) {
        cout << "Blockchain is valid." << endl;
    } else {
        cout << "Blockchain is invalid." << endl;
    }

    // Delete a block
    if (blockchain.deleteBlock("hash2")) {
        cout << "Block with hash2 deleted successfully." << endl;
    } else {
        cout << "Block with hash2 not found." << endl;
    }

    // Print the blockchain again
    blockchain.printChain();

    return 0;
}