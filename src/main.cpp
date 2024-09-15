#include <iostream>
#include <string>

using namespace std;

// A structure to represent each block in the blockchain
struct Block {
    Block* next;
    Block* prev;
    string prevHash; 
    string Hash;
    int nonce;
    string transactions;

    Block(string hash, int Nonce, string transactions) {
        this->Hash = hash;
        this->nonce = Nonce;
        this->transactions = transactions; 
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
    void AppendBlock(string insertHash, int insertNonce, string insertTransactions) {
        Block* newNode = new Block(insertHash, insertNonce, insertTransactions);

        if (head == nullptr) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }

        cout << "Successfully inserted block with hash: " << insertHash << endl;
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
    blockchain.AppendBlock("hash1", 1, "2023-01-01T12:00:00Z");
    blockchain.AppendBlock("hash2", 2, "2023-02-01T12:00:00Z");
    blockchain.AppendBlock("hash3", 3, "2023-03-01T12:00:00Z");

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