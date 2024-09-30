#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "blockchain.h"   
#include "db.h"           

using namespace std;

int main() {

    BlockChain blockchain;

    cout <<"\n";
    
    if (databaseToChain(blockchain)){
        blockchain.printChain();
    }

    string firstName, lastName, vote;

    cout << "Enter your first name: ";
    cin >> firstName;
    cout << "Enter your last name: ";
    cin >> lastName;
    cout << "Enter your vote (Candidate name): ";
    cin.ignore(); 
    getline(cin, vote);

    cout << "Mining block for your vote..." << endl;
    auto start = chrono::high_resolution_clock::now();
    blockchain.AppendBlock("Vote: " + vote);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;
    cout << "Block for your vote took: " << elapsed.count() << " seconds to mine" << endl;

    
    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB(firstName, lastName, vote);

    
    cout << "Is blockchain valid? " << (blockchain.validateChain() ? "Yes" : "No") << endl;
    
    return 0;
}