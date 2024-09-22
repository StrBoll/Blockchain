#ifndef db_h
#define db_h

#include <pqxx/pqxx>
#include <string> 


void blockDB(const std::string &prevHash, const std::string &hash, int nonce, time_t transactions);


#endif