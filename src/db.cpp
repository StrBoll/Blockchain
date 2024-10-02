#include "db.h"
#include <pqxx/pqxx>
#include <iostream>
#include "blockchain.h"
#include <string>

using namespace pqxx;
using namespace std;


void insertBlockDB(const std::string& prevHash, const std::string& hash, int nonce, time_t transactions) {
    try {
        
        connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");
        
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
            connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");


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
    connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");

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
            connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");


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
        connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");

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
            connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");


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

string tallyVotes(){

    try {

        connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");


        int Donald(0);
        int Kamala(0);
        int Jill(0);
        int Phillip(0);
        if (C.is_open()){
            nontransaction N(C);

            string command = "SELECT candidate_vote FROM votes;";

            result R(N.exec(command));
            
            for (auto iter = R.begin(); iter != R.end(); ++iter){
                
                if (iter[0].as<string>() == "Phillip Boll"){
                    Phillip++;
                } else if (iter[0].as<string>() == "Kamala Harris"){
                    Kamala++;
                } else if (iter[0].as<string>() == "Donald Trump"){
                    Donald++;
                } else {
                    Jill++;
                }



            }

            int highestVotes = max({Phillip, Kamala, Donald, Jill});
            string candidate;

            if (highestVotes == Phillip){
                candidate = "Phillip Boll";
            }
            else if (highestVotes == Kamala){
                candidate = "Kamala Harris";
            }
            else if (highestVotes == Donald){
                candidate = "Donald Trump";
            }
            else {

                candidate = "Jill Stein";
            }
            

            


            return "Top Candidate: " + candidate + " with " + to_string(highestVotes) + " votes";






           
            
        }

        

    }

    catch (const exception &e){
        cerr << e.what() << endl;
        return "Could not connect to AWS database";
    }
}
