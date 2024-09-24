#include "db.h"
#include <pqxx/pqxx> 
#include <iostream>
#include <string>

using namespace pqxx;
using namespace std;
void blockDB(const std::string& prevHash, const std::string& hash, int nonce, time_t transactions) {
    try {
        
        connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");
        
        if (C.is_open()) {
            work W(C);
            string sql = "INSERT INTO blocks (previous_hash, current_hash, nonce, transaction_time) "
            "VALUES ('" + prevHash + "', '" + hash + "', " + to_string(nonce) + ", to_timestamp(" + to_string(transactions) + "));";

            
            W.exec(sql);
            W.commit();

        } 

    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}
