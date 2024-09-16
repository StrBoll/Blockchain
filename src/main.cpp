#include <iostream>
#include <string>
#include <ctime>
#include <sstream>
using namespace std;

// A structure to represent each block in the blockchain
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
    }
};

// Class to manage the blockchain (doubly linked list of blocks)
class BlockChain {
private:
    Block* head;
    Block* tail;

public: 
    // Constructor initializes an empty blockchain
    BlockChain() {
        head = nullptr;
        tail = nullptr;
    }

    // Append a block to the end of the blockchain
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


        // BELOW CODE IS SIMULATION OF HASH CREATION : TEMPORARY UNTIL WE HAVE IMPLEMENTED HASHING SYSTEM:

        stringstream temporary;
        temporary << newBlock->transactions << newBlock->nonce;
        newBlock->Hash = temporary.str();
        cout << "Block added with hash: " << newBlock->Hash << endl;


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