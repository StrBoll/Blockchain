#include "crow.h"
#include "blockchain.h"
#include "db.h"

BlockChain blockchain;  // Instantiate your blockchain object

int main() {
    crow::SimpleApp app;

    // Default route for testing
    CROW_ROUTE(app, "/")([]() {
        return "Hello, Crow!";
    });

    // Route to append a block to the blockchain
    CROW_ROUTE(app, "/appendBlock").methods(crow::HTTPMethod::POST)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        
        if (!body) {
            return crow::response(400, "Invalid JSON data");
        }

        std::string firstName = body["firstName"].s();
        std::string lastName = body["lastName"].s();
        std::string candidate = body["candidate"].s();

        // Create a data string for the block
        std::string blockData = firstName + " " + lastName + " voted for " + candidate;

        // Append block to blockchain
        blockchain.AppendBlock(blockData);

        // Insert the vote into the database
        insertVoteDB(firstName, lastName, candidate);

        return crow::response(200, "Block added to blockchain and vote saved.");
    });

    app.port(18080).multithreaded().run();
}
