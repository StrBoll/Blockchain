#include "db.h"
#include <pqxx/pqxx> 
#include <iostream>
#include <string>

using namespace pqxx;
using namespace std;


void insertBlockDB(const std::string& prevHash, const std::string& hash, int nonce, time_t transactions) {
    try {
        
        connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");
        
        if (C.is_open()) {
            work W(C);
            string data = "INSERT INTO blocks (previous_hash, current_hash, nonce, transaction_time) "
            "VALUES ('" + prevHash + "', '" + hash + "', " + to_string(nonce) + ", to_timestamp(" + to_string(transactions) + "));";

            
            W.exec(data);
            W.commit();

        } 

    } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}



bool blocksPresent(){
        try {
            connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");

            if (C.is_open()){
                nontransaction N(C);
                string Data = "SELECT block_number FROM blocks;";
                result R(N.exec(Data));

                if (R.size() == 0){
                    return false;
                }




            
            }

            return true;

        } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}


bool eraseDB(){

    try {
    connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");
    if (C.is_open()){

        work W(C);
        string command = "DELETE FROM blocks";

        W.exec(command);
        W.commit();
        
        
        return true;


    } else {
        cout << "Could not connect to the database." << endl;
    }
    }catch (const exception &e) {
        cerr << e.what() << endl;
    }
} 
