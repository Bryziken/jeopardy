#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <cstdlib>

using namespace std;

// Decode percent-encoded characters (e.g., %20 => space)
string urlDecode(const string& str) {
    string result;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.size()) {
            string hex = str.substr(i + 1, 2);
            char decodedChar = static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            result += decodedChar;
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

// Parse application/x-www-form-urlencoded into key-value pairs
map<string, string> parseFormData(const string& data) {
    map<string, string> result;
    stringstream ss(data);
    string pair;
    while (getline(ss, pair, '&')) {
        size_t eq = pair.find('=');
        if (eq != string::npos) {
            string key = urlDecode(pair.substr(0, eq));
            string value = urlDecode(pair.substr(eq + 1));
            result[key] = value;
        }
    }
    return result;
}

int main() {
    cout << "Content-type: text/html\n\n";

    string method = getenv("REQUEST_METHOD") ? getenv("REQUEST_METHOD") : "";
    if (method != "POST") {
        cout << "<p>Error: Only POST method supported.</p>";
        return 1;
    }

    string contentLengthStr = getenv("CONTENT_LENGTH") ? getenv("CONTENT_LENGTH") : "";
    size_t contentLength = 0;
    try {
        contentLength = static_cast<size_t>(stoi(contentLengthStr));
    } catch (...) {
        cout << "<p>Error: Invalid content length.</p>";
        return 1;
    }

    string postData(contentLength, '\0');
    cin.read(&postData[0], contentLength);

    map<string, string> formData = parseFormData(postData);

    if (formData.find("numTeams") == formData.end()) {
        cout << "<h2 style='color: red;'>Missing 'numTeams' input.</h2>";
        return 1;
    }

    int numTeams = 0;
    try {
        numTeams = stoi(formData["numTeams"]);
    } catch (...) {
        cout << "<h2 style='color: red;'>Invalid number format for teams.</h2>";
        return 1;
    }

    if (numTeams < 2 || numTeams > 9) {
        cout << "<h2 style='color: red;'>Number of teams must be between 2 and 9.</h2>";
        return 1;
    }

    // Output form for team names
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
    cout << "</form></body></html>";

    return 0;
}
