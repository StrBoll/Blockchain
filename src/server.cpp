#include "crow.h"
#include "blockchain.h"
#include "db.h"

BlockChain blockchain;

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.write("Hello, Crow!");
        return res;
    });

    CROW_ROUTE(app, "/appendBlock").methods(crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS)
    ([](const crow::request& req) {
        if (req.method == crow::HTTPMethod::OPTIONS) {
            crow::response res;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.code = 204;
            return res;
        }

        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON data");
        }

        std::string firstName = body["firstName"].s();
        std::string lastName = body["lastName"].s();
        std::string candidate = body["candidate"].s();
        std::string blockData = firstName + " " + lastName + " voted for " + candidate;

        blockchain.AppendBlock(blockData);

        insertBlockDB(blockchain.getTail()->prevHash, blockchain.getTail()->Hash, blockchain.getTail()->nonce, blockchain.getTail()->transactions);
        insertVoteDB(firstName, lastName, candidate);

        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.write("Block added to blockchain and vote saved.");
        res.code = 200;
        return res;
    });

    app.port(18080).multithreaded().run();
}
