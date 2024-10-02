#include <drogon/drogon.h>
#include "blockchain.h"
#include "db.h"
#include <pqxx/pqxx>

BlockChain blockchain;

using namespace drogon;
using namespace pqxx;
using namespace std;


int main() {
   
    app().registerHandler("/appendBlock", [](const HttpRequestPtr &req, function<void(const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }

   
    string firstName = (*json)["firstName"].asString();
    string lastName = (*json)["lastName"].asString();
    string candidate = (*json)["candidate"].asString();
    string blockData = firstName + " " + lastName + " voted for " + candidate;

    
    string miningResult;
    blockchain.AppendBlock(blockData, miningResult);  
    
    
    insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
    insertVoteDB(firstName, lastName, candidate);

    
    auto resp = HttpResponse::newHttpResponse();
    resp->addHeader("Access-Control-Allow-Origin", "*");
    resp->setBody(miningResult);  
    callback(resp);
});

    // functions from my terminal UI
    app().registerHandler("/printVotes", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
        string toTerminal;
        try {
            connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");
            if (C.is_open()) {
                nontransaction N(C);
                string command = "SELECT * FROM votes;";
                result R(N.exec(command));

                for (auto iter = R.begin(); iter != R.end(); ++iter) {
                    toTerminal += iter["voterFirst"].as<string>() + " " + iter["voterLast"].as<string>() + " Vote: " + iter["candidate_vote"].as<string>() + "\n";
                }
            } else {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody("Failed to connect to AWS Database");
                callback(resp);
                return;
            }
        } catch (const exception &e) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k500InternalServerError);
            resp->setBody("Error retrieving votes");
            callback(resp);
            return;
        }

        auto resp = HttpResponse::newHttpResponse();
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->setBody(toTerminal);
        callback(resp);
    });

    app().registerHandler("/topCandidate", [](const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    
        string topPerson = "\n" + tallyVotes();
        
        auto resp = HttpResponse::newHttpResponse();
        resp->addHeader("Access-Control-Allow-Origin", "*");  
        resp->setBody(topPerson);
        callback(resp);
    });



    
    app().addListener("0.0.0.0", 18080).run();
}
