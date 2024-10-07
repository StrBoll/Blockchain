#include "multithread.h"
#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <chrono>
#include "openssl/sha.h"


using namespace std;


Mining::Mining(int numThreads, int difficulty){
    this->hashFound = false;
    this->nonceFound = -1;
    this->numThreads = numThreads;
    this->difficulty = difficulty;


}

void Mining::threading(int threadNum, atomic<int>& sharedNonce, const string& prevHash, const string& data, string& resultMessage, int& totalHashTime) {
    string tempHash;
    
    auto startTimer = chrono::high_resolution_clock::now();
    while (!done.load()) {  
        int nonce = sharedNonce.fetch_add(1);  
        string convert = prevHash + data + to_string(nonce);
        string tempHash = compute256(convert);

        if (validHash(tempHash)) {
            lock_guard<mutex> lock(resultMtx);  
            if (!done.load()) {
                auto endTimer = chrono::high_resolution_clock::now();  
                auto totalTime = chrono::duration_cast<std::chrono::milliseconds>(endTimer - startTimer);
                totalHashTime = static_cast<int>(totalTime.count()); 
                done = true;
                nonceFound = nonce;
                hashFound = tempHash;
                resultMessage = "Thread #" + to_string(threadNum) + " found valid nonce at: " + to_string(nonce) + "\n Hash: " + tempHash + "\n This operation took " + to_string(totalHashTime) + " seconds.";

                cout << "Thread #" << threadNum << " found valid nonce at: " << nonce 
                     << " with hash: " << tempHash << " with time: " << totalHashTime << endl;
            }
        }
    }
}

int Mining::mineBlock(string prevHash, string data, string& returnHash, string& resultMessage, int& totalHashTime){
    vector<thread> threads;
    atomic<int> sharedNonce(0);
    int nonce = 0;
    done = false; 

    

    for (int i = 0; i < numThreads; ++i){
        threads.emplace_back(&Mining::threading, this, i, ref(sharedNonce), cref(prevHash), cref(data), ref(resultMessage), ref(totalHashTime));
    }

    for (auto &thread : threads){
        thread.join();
    }

    returnHash = hashFound;
    return nonceFound;


}

string Mining::HexIt(const unsigned char* input, size_t length) {
    const char* hexAlphabet = "0123456789abcdef";
    string output;
    
    for (size_t i = 0; i < length; i++) {
        output.push_back(hexAlphabet[(input[i] >> 4) & 0xF]);
        output.push_back(hexAlphabet[input[i] & 0xF]);       
    }
    
    return output;
}

bool Mining::validHash(const string& hash){
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


string Mining::compute256(const string& input){

// Deprecated, newer Envelope API is available but since this is just a project I'm doing for learning about blockchain I figured it didn't matter
   unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(hash, &sha256);

    

    return HexIt(hash, SHA256_DIGEST_LENGTH);
}