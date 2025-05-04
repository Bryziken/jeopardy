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

    // Output game page
    cout <<
        "Content-Type: text/html; charset=UTF-8\r\n\r\n"
        "<!DOCTYPE html>\n"
        "<html lang=\"en\">\n"
        "<head>\n"
        "  <meta charset=\"UTF-8\">\n"
        "  <meta name=\"viewport\" content=\"width=device-width,initial-scale=1\">\n"
        "  <title>Jeopardy!</title>\n"
        "  <style>\n"
        "    body { background-color: #001f3f; color: white; font-family: sans-serif; text-align: center; }\n"
        "    .team-counter { display: inline-block; margin: 15px; padding: 15px; background: #003366;\n"
        "                    border: 2px solid #fff; border-radius: 10px; color: gold; min-width: 120px; }\n"
        "    .score-display { font-size: 2rem; margin: 10px 0; }\n"
        "    .score-buttons button { margin: 5px; padding: 5px 10px; font-size: 1.2rem;\n"
        "                            background-color: #0074D9; color: white; border: none; border-radius: 5px;\n"
        "                            cursor: pointer; transition: background-color 0.3s; }\n"
        "    .score-buttons button:hover { background-color: #39CCCC; }\n"
        "  </style>\n"
        "</head>\n"
        "<body>\n"
        "  <header><h1 style=\"color:gold; text-shadow:2px 2px #000;\">Jeopardy!</h1></header>\n"
        "  <div id=\"scoreboard\" style=\"display:flex;flex-wrap:wrap;justify-content:center;\">\n";

    for (int i = 0; i < numTeams && i < (int)teamNames.size(); ++i) {
        cout <<
        "    <div class=\"team-counter\">\n"
        "      <h2>" << teamNames[i] << "</h2>\n"
        "      <div class=\"score-display\" id=\"score" << i << "\">0</div>\n"
        "      <div class=\"score-buttons\">\n"
        "        <button onclick=\"changeScore(" << i << ", -100)\">-100</button>\n"
        "        <button onclick=\"changeScore(" << i << ", +100)\">+100</button>\n"
        "      </div>\n"
        "    </div>\n";
    }

    cout <<
        "  </div>\n"
        "  <script>\n"
        "    function changeScore(idx, delta) {\n"
        "      const el = document.getElementById('score' + idx);\n"
        "      let val = parseInt(el.textContent, 10) || 0;\n"
        "      el.textContent = val + delta;\n"
        "    }\n"
        "  </script>\n"
        "</body>\n"
        "</html>\n";

    return 0;
}
