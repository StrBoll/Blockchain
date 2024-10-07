#ifndef MULTITHREAD_H
#define MULTITHREAD_H

#include <string>
#include <atomic>
#include <mutex>

using namespace std;


class Mining {
public:
    // Constructor
    Mining(int numThreads, int difficulty);

    
    int mineBlock(string prevHash, string data, string& returnHash, string& resultMessage, int& totalHashTime);

private:
    
    atomic<bool> done;
    mutex resultMtx;
    int nonceFound;
    string hashFound;
    int numThreads;
    int difficulty;

    void threading(int threadNum, atomic<int>& sharedNonce, const string& prevHash, const string& data, string& resultMessage, int& totalHashTime);
    string compute256(const string& input);
    bool validHash(const std::string& hash);
    string HexIt(const unsigned char* input, size_t length);
};







#endif 