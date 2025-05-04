// jeopardy.cpp
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <cctype>

using namespace std;

// URL-decode utility
string decodeURL(const string &in) {
    string out;
    char hex[3] = {0};
    for (size_t i = 0; i < in.size(); ++i) {
        if (in[i] == '%' && i + 2 < in.size() &&
            isxdigit(in[i+1]) && isxdigit(in[i+2])) {
            hex[0] = in[i+1];
            hex[1] = in[i+2];
            out += static_cast<char>(strtol(hex, nullptr, 16));
            i += 2;
        }
        else if (in[i] == '+') {
            out += ' ';
        }
        else {
            out += in[i];
        }
    }
    return out;
}

// Parse URL-encoded form data into key/value pairs
vector<pair<string, string>> parse_form(const string &body) {
    vector<pair<string, string>> fields;
    istringstream ss(body);
    string pair;
    while (getline(ss, pair, '&')) {
        auto eq = pair.find('=');
        if (eq != string::npos) {
            string key = decodeURL(pair.substr(0, eq));
            string value = decodeURL(pair.substr(eq+1));
            fields.emplace_back(key, value);
        }
    }
    return fields;
}

int main() {
    // Read POST body
    const char* len_str = getenv("CONTENT_LENGTH");
    int len = len_str ? atoi(len_str) : 0;
    string body;
    body.resize(len);
    cin.read(&body[0], len);

    // Parse form
    auto fields = parse_form(body);

    // Get numInputs
    int numTeams = 0;
    for (auto &kv : fields) {
        if (kv.first == "numInputs") {
            numTeams = atoi(kv.second.c_str());
            break;
        }
    }

    // Validate
    if (numTeams < 2 || numTeams > 9) {
        // Error page
        cout <<
            "Content-Type: text/html; charset=UTF-8\r\n\r\n"
            "<!DOCTYPE html>\n"
            "<html><head><meta charset=\"UTF-8\"><title>Invalid Teams</title></head>\n"
            "<body style=\"background:#001f3f;color:white;font-family:sans-serif;text-align:center;padding:50px;\">\n"
            "  <h1 style=\"color:red;\">Error: You must enter between 2 and 9 teams.</h1>\n"
            "  <p><a href=\"../CPS3525/startPage.html\" style=\"color:gold;\">Go back to Team Setup</a></p>\n"
            "</body></html>\n";
        return 0;
    }

    // Collect team names
    vector<string> teamNames;
    for (int i = 1; i <= numTeams; ++i) {
        string key = "team" + to_string(i);
        for (auto &kv : fields) {
            if (kv.first == key) {
                teamNames.push_back(kv.second);
                break;
            }
        }
    }

    
    return 0;
}
