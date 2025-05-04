#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>  // For getenv
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>

using namespace std;
using namespace cgicc;

// Define a structure for teams
struct Team {
    string name;
    int score;
};

int main() {
    try {
        Cgicc cgi;

        // 1. Get the number of teams from the form input
        const_form_iterator it = cgi.getElement("teamCount");
        if (it == cgi.getElements().end()) {
            cout << "<h1>Error: Team count missing.</h1></body></html>";
            return 1;
        }

        int numTeams = stoi(it->getValue());
        vector<Team> teams;

        // 2. Get the team names and initialize their scores
        for (int i = 1; i <= numTeams; ++i) {
            string key = "team" + to_string(i);
            const_form_iterator teamIt = cgi.getElement(key);
            if (teamIt != cgi.getElements().end()) {
                Team team;
                team.name = teamIt->getValue();
                team.score = 0;  // Set the initial score to 0
                teams.push_back(team);
            } else {
                cout << "<h1>Error: Missing name for " << key << "</h1></body></html>";
                return 1;
            }
        }

        // 3. Construct the JSON output for scores
        string json = "[\n";
        for (size_t i = 0; i < teams.size(); ++i) {
            json += "  {\n";
            json += "    \"name\": \"" + teams[i].name + "\",\n";
            json += "    \"score\": " + to_string(teams[i].score) + "\n";
            json += "  }";
            if (i < teams.size() - 1) json += ",";
            json += "\n";
        }
        json += "]";

        // 4. Save JSON data to file for future use (optional)
        ofstream outFile("/home/student/ortegbry/public_html/cgi-bin/scores.json");
        if (outFile.is_open()) {
            outFile << json;
            outFile.close();
        } else {
            cerr << "Error: Could not write to scores.json" << endl;
        }

        // 5. Output the redirect to jeopardy.html
        cout << "Content-type: text/html\r\n";  // HTTP header for HTML content
        cout << "Location: https://obi.kean.edu/~ortegbry@kean.edu/CPS3525/jeopardy.html\r\n\r\n";

        // Redirect to jeopardy.html after processing
        return 0;

    } catch (exception& e) {
        // Handle exceptions gracefully
        cout << "Content-type: text/plain\n\n";
        cout << "Error: " << e.what() << endl;
        return 1;
    }
}
