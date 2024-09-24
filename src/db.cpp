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


string getLatestHash(){
    try {
            connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");

            if (C.is_open()){
                nontransaction N(C);
                

                string lastBlock = "SELECT current_hash FROM blocks ORDER BY block_number DESC LIMIT 1;";

                result latestHash(N.exec(lastBlock));

                if (!latestHash.empty() && !latestHash[0]["current_hash"].is_null()) {
                return latestHash[0]["current_hash"].as<string>();
            } else {
                cout << "No block found or current_hash is NULL." << endl;
                return "";  
            }





            
            }

            

        } catch (const exception &e) {
        cerr << e.what() << endl;
    }
}


int lastBlock(){
    try {
        connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");
        if (C.is_open()){
            nontransaction N(C);

            string command = "SELECT block_number FROM blocks ORDER BY block_number DESC LIMIT 1;";
            result result1(N.exec(command));

            if (!result1.empty()){
                int lastNum = result1[0]["block_number"].as<int>();
                return lastNum;
            } else {
                cerr << "No blocks in database yet" << endl;
                return -1; 
            }   
        }

    }
    catch (const exception &e){
        cerr << e.what() << endl;
        return -1;
    }
}


void insertVoteDB(const std::string& voter_first, const std::string& voter_last, const std::string& candidate){
    string blockHash = getLatestHash();

    if (blockHash != ""){
    
        try {
            connection C("dbname=database user=phillipboll3 password=Internship2025 hostaddr=127.0.0.1 port=5432");

            if (C.is_open()){
                work W(C);

                string command = "INSERT INTO votes (vote_hash, voter_registration, voterFirst, voterLast, candidate_vote) "
                                "VALUES ('" + blockHash + "', '0000', '" + voter_first + "', '" + voter_last + "', '" + candidate + "');";
                W.exec(command);
                W.commit();

                
            }
        }

        catch (const exception &e){
            cerr << e.what() << endl;
        }
    } else {
        cerr << "Last block's hash not found" << endl;
    }

}


