#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "blockchain.h"   // Include the blockchain class header
#include "db.h"           // Include the database-related functions

using namespace std;

int main() {

    BlockChain blockchain;

    // Repopulate the blockchain from the database
    if (databaseToChain(blockchain)){
        blockchain.printChain();
    }


    

    
    // Mining block 1
    cout << "Mining block 1... " << endl;
    auto start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Candidate X");
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Block 1 took: " << elapsed.count() << " seconds to mine" << endl;

    // Add block 1 to database
    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB("phillip", "Boll", "Candidate X");

    // Mining block 2
    cout << "Mining block 2... " << endl;
    start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Candidate Y");
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Block 2 took: " << elapsed.count() << " seconds to mine" << endl;

    // Adjust the difficulty
    cout << "Previous Difficulty: " << blockchain.difficulty << endl;
    blockchain.difficulty = adjustDifficulty(blockchain.getTail()->prev, blockchain.getTail(), blockchain.difficulty, 5);
    cout << "Updated Difficulty: " << blockchain.difficulty << endl;

    // Add block 2 to database
    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB("Karina", "Santos", "Candidate Y");

    // Mining block 3
    cout << "Mining block 3... " << endl;
    start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: Barack Obama");
    end = chrono::high_resolution_clock::now();
    elapsed = end - start;
    cout << "Block 3 took: " << elapsed.count() << " seconds to mine" << endl;

    // Add block 3 to database
    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB("Adrian", "Kolber", "Candidate Z");

    // Validate the blockchain
    cout << "Is blockchain valid? " << (blockchain.validateChain() ? "Yes" : "No") << endl;
    
    

    return 0;
}
