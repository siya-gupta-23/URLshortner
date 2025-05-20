#include <iostream>
#include <unordered_map>
#include <string>
#include <random>
#include <winsock2.h>   // Windows socket support
#include <ws2tcpip.h>   // Additional Windows networking support
#include "cpp-httplib/httplib.h"
#include "json/json.hpp"

using namespace std;
using namespace httplib;
using json = nlohmann::json;

// memory map for short -> long URL
unordered_map<string, string> url_db;

// Random short code generator
string generate_short_code(int length = 6) {
    static const string chars =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static mt19937 rng{random_device{}()};
    uniform_int_distribution<> dist(0, chars.size() - 1);

    string code;
    for (int i = 0; i < length; ++i)
        code += chars[dist(rng)];

    return code;
}

// Add CORS headers (frontend)
void add_cors_headers(Response &res) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {  // Initialize Windows Sockets
        std::cerr << "WSAStartup failed!" << std::endl;
        return 1;
    }

    Server svr;

    // Handle CORS preflight
    svr.Options(R"(.*)", [](const Request &, Response &res) {
        add_cors_headers(res);
        res.status = 204;
    });

    //POST /shorten is an API endpoint where users can send a long URL in the request body (as JSON) and get back a shortened URL.


    svr.Post("/shorten", [](const Request &req, Response &res) {
        add_cors_headers(res);
        try {
            auto data = json::parse(req.body);
            string long_url = data["url"];
            string short_code = generate_short_code();
            url_db[short_code] = long_url;

            json response_json = {
                {"short_url", "http://localhost:8080/" + short_code}
            };

            res.set_content(response_json.dump(), "application/json");
            cout << "Shortened: " << long_url << " -> " << short_code << endl;
        } catch (...) {
            res.status = 400;
            res.set_content(R"({"error": "Invalid JSON or request."})", "application/json");
        }
    });

    // GET --> <shortcode>
    svr.Get(R"(/([a-zA-Z0-9]+))", [](const Request &req, Response &res) {
        add_cors_headers(res);
        string code = req.matches[1];
        if (url_db.find(code) != url_db.end()) {
            cout << "Redirecting " << code << " -> " << url_db[code] << endl;
            // Convert std::string to const char* using c_str() 
            res.set_redirect(url_db[code].c_str());
        } else {
            res.status = 404;
            res.set_content("Short URL not found", "text/plain");
        }
    });
    

    cout << "Server running on http://localhost:8080..." << endl;
    svr.listen("localhost", 8080);

    WSACleanup();  // Clean up Winsock

    return 0;
}

