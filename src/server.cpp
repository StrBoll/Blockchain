#include <drogon/drogon.h>
#include "blockchain.h"
#include "db.h"
#include <pqxx/pqxx>

BlockChain blockchain;

using namespace drogon;
using namespace pqxx;
using namespace std;

int main() {
    // Define the route for appending a block
    drogon::app().registerHandler("/appendBlock", [](const HttpRequestPtr &req,
                                           std::function<void(const HttpResponsePtr &)> &&callback) {
        auto json = req->getJsonObject();
        if (!json) {
            auto resp = HttpResponse::newHttpResponse();
            resp->setStatusCode(k400BadRequest);
            callback(resp);
            return;
        }

        // Extract JSON data
        std::string firstName = (*json)["firstName"].asString();
        std::string lastName = (*json)["lastName"].asString();
        std::string candidate = (*json)["candidate"].asString();
        std::string blockData = firstName + " " + lastName + " voted for " + candidate;

        // Append block to blockchain
        blockchain.AppendBlock(blockData);
        insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
        insertVoteDB(firstName, lastName, candidate);

        // Respond to the request
        auto resp = HttpResponse::newHttpJsonResponse(*json);
        resp->addHeader("Access-Control-Allow-Origin", "*");
        callback(resp);
    });

    // Define the route to print votes from the database
    drogon::app().registerHandler("/printVotes", [](const HttpRequestPtr &req,
                                          std::function<void(const HttpResponsePtr &)> &&callback) {
        std::string toTerminal;
        try {
            connection C("dbname=aws_database user=phillipboll3 password='#NewPassword2024' host=localhost port=5432");
            if (C.is_open()) {
                nontransaction N(C);
                std::string command = "SELECT * FROM votes;";
                result R(N.exec(command));

                for (auto iter = R.begin(); iter != R.end(); ++iter) {
                    toTerminal += iter["voterFirst"].as<std::string>() + " " + iter["voterLast"].as<std::string>() + " Vote: " + iter["candidate_vote"].as<std::string>() + "\n";
                }
            } else {
                auto resp = HttpResponse::newHttpResponse();
                resp->setStatusCode(k500InternalServerError);
                resp->setBody("Failed to connect to AWS Database");
                callback(resp);
                return;
            }
        } catch (const std::exception &e) {
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

    // Start the server
    drogon::app().addListener("0.0.0.0", 18080).run();
}
