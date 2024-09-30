#include "crow.h"
#include "blockchain.h"
#include "db.h"

BlockChain blockchain;  // Instantiate your blockchain object

int main() {
    crow::SimpleApp app;

    // Default route for testing
    CROW_ROUTE(app, "/")([]() {
        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");  // Allow CORS from any origin
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.write("Hello, Crow!");
        return res;
    });

    // Route to append a block to the blockchain
    CROW_ROUTE(app, "/appendBlock").methods(crow::HTTPMethod::POST, crow::HTTPMethod::OPTIONS)
    ([](const crow::request& req) {
        // Handle preflight OPTIONS request
        if (req.method == crow::HTTPMethod::OPTIONS) {
            crow::response res;
            res.set_header("Access-Control-Allow-Origin", "*");
            res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
            res.set_header("Access-Control-Allow-Headers", "Content-Type");
            res.code = 204;  // No content for OPTIONS request
            return res;
        }

        // Handle POST request
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

        // Return success response with CORS headers
        crow::response res;
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.write("Block added to blockchain and vote saved.");
        res.code = 200;  // Success
        return res;
    });

    app.port(18080).multithreaded().run();
}
