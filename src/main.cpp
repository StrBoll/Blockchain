#include <iostream>
#include <algorithm> 
#include <string> 
#include <sstream>
#include <iomanip>


using namespace std;


// implement a doubly-linked list that acts as the blockchain 
struct Block {
    Block* next;
    Block* prev;
    string prevHash; 
    string Hash;
    int nonce;
    string transactions;

    Block(string hash, int Nonce, string timestamp){
        this->Hash = hash;
        this->nonce = Nonce;
        this->transactions = timestamp; // timestamp is in ISO 8601 FORMAT (More on this in README)
        next = nullptr;
        prev = nullptr;
    }
    


};



class BlockChain {

    private:
        
        Block* head;
        Block* tail;


    public: 

        BlockChain(){ // default constructor 
            head = nullptr;
            tail = nullptr;


        }

        void AppendBlock(){ // adds a block to the block chain at the very end
            
        }

        Block* RetrieveBlock(){ // finds pointer that contains block address for a block with a specific hash
            return head;
        }

        bool validateChain(){  // returns true or false based on whether the previous hash matches in the block chain
            return 0;
        }




};

int main(){



    



}