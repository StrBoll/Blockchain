#ifndef db_h
#define db_h

#include <pqxx/pqxx>
#include <string> 


void insertBlockDB(const std::string &prevHash, const std::string &hash, int nonce, time_t transactions); // inserts new blocks to database

bool blocksPresent(); // checks if blocks are in blockchain 

bool eraseDB(); // erase everything in database

std::string getLatestHash();


#endif