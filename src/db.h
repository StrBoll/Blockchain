#ifndef db_h
#define db_h

#include <pqxx/pqxx>
#include <string> 


void insertBlockDB(const std::string &prevHash, const std::string &hash, int nonce, time_t transactions);

bool blocksPresent();

bool eraseDB();


#endif