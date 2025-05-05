#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <algorithm>  

using namespace std;

string getScoreBoardHTML(vector<string> teamNames, int numTeams) {
    // Output HTML header
    cout << "Content-type: text/html\n\n";

    // Get the number of teams from environment variable (GET or POST)
    string divHTML = "";

    size_t pos = 0;
    while ((pos = input.find('&')) != string::npos) {
        string token = input.substr(0, pos);
        size_t eq = token.find('=');
        string key = token.substr(0, eq);
        string val = token.substr(eq + 1);

        if (key == "teamCount") {
            numTeams = stoi(val);
        } else if (key.rfind("team", 0) == 0) {
            replace(val.begin(), val.end(), '+', ' ');  // Handle spaces
            teamNames.push_back(val);
        }

        input.erase(0, pos + 1);
    }

    // Parse the last parameter (if not already consumed)
    if (!input.empty()) {
        size_t eq = input.find('=');
        string key = input.substr(0, eq);
        string val = input.substr(eq + 1);
        if (key.rfind("team", 0) == 0) {
            replace(val.begin(), val.end(), '+', ' ');
            teamNames.push_back(val);
        }
    }

    // Output the HTML divs

    for (int i = 0; i < numTeams && i < (int)teamNames.size(); ++i) {
        divHTML.append(
            "  <div class=\"team-counter\">\n"
            "    <h2>" << teamNames[i] << "</h2>\n"
            "    <div class=\"score-display\" id=\"score" << i << "\">0</div>\n"
            "    <div class=\"score-buttons\">\n"
            "      <button onclick=\"changeScore(this, -100)\">-100</button>\n"
            "      <button onclick=\"changeScore(this, 100)\">+100</button>\n"
            "    </div>\n"
            "  </div>\n");  
    }
    divHTML.append("</div>");

    

    return divHTML;
}
