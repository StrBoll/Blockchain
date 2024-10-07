#ifndef db_h
#define db_h

#include <pqxx/pqxx>
#include <string> 
#include "blockchain.h"


void insertBlockDB(const std::string &prevHash, const std::string &hash, int nonce, time_t transactions, int miningTime); // inserts new blocks to database

bool blocksPresent(); // checks if blocks are in blockchain 

bool eraseDB(); // erase everything in database

void insertVoteDB(const std::string& voter_first, const std::string& voter_last, const std::string& candidate);

int lastBlock();

std::string getLatestHash();

int verifyVoters();

std::string tallyVotes();

int averageHashTime();

#endif