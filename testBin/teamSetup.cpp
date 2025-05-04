#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;

// Utility to get environment variable or empty string
string getEnv(const string& key) {
    const char* val = getenv(key.c_str());
    return val ? string(val) : "";
}

// Reads stdin content
string readStdin(size_t length) {
    string data(length, '\0');
    cin.read(&data[0], length);
    return data;
}

// Extracts value for a key in url-encoded form data
string extractFormField(const string& data, const string& key) {
    size_t start = data.find(key + "=");
    if (start == string::npos) return "";
    start += key.length() + 1;
    size_t end = data.find("&", start);
    return data.substr(start, end - start);
}

int main() {
    cout << "Content-type: text/html\n\n";

    string method = getEnv("REQUEST_METHOD");
    if (method != "POST") {
        cout << "<p>Error: Only POST method is supported.</p>";
        return 1;
    }

    string contentType = getEnv("CONTENT_TYPE");
    string contentLengthStr = getEnv("CONTENT_LENGTH");

    if (contentType.find("multipart/form-data") != string::npos) {
        // Not implemented: parsing multipart form data (advanced)
        cout << "<p>Error: File upload via multipart/form-data is not yet supported in this version.</p>";
        return 1;
    }

    // Handle x-www-form-urlencoded POST
    size_t contentLength = atoi(contentLengthStr.c_str());
    string postData = readStdin(contentLength);

    string numTeamsStr = extractFormField(postData, "numTeams");
    int numTeams = atoi(numTeamsStr.c_str());

    if (numTeams < 2 || numTeams > 9) {
        cout << "<h1 style='color: red;'>Invalid number of teams. Please enter a value between 2 and 9.</h1>";
        return 1;
    }

    // Output the next HTML form
    cout << "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>Team Names</title>";
    cout << "<style>body { background-color: #001f3f; color: gold; font-family: sans-serif; text-align: center; padding: 50px; }";
    cout << "input { font-size: 1.2rem; margin: 10px; padding: 8px; border-radius: 5px; border: none; width: 250px; }";
    cout << "button { font-size: 1.2rem; padding: 10px 20px; background-color: #0074D9; color: white; border: none; border-radius: 8px; cursor: pointer; }";
    cout << "button:hover { background-color: #39CCCC; }</style></head><body>";

    cout << "<h1>Enter Team Names</h1>";
    cout << "<form action='/cgi-bin/scorePage.cgi' method='post'>";
    for (int i = 1; i <= numTeams; ++i) {
        cout << "<input type='text' name='team" << i << "' placeholder='Team " << i << " Name' required><br>";
    }
    cout << "<input type='hidden' name='teamCount' value='" << numTeams << "'>";
    cout << "<button type='submit'>Continue to Game</button>";
    cout << "</form>";

    cout << "</body></html>";
    return 0;
}
