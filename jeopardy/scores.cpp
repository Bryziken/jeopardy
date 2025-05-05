#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>  // For getenv
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>

using namespace std;
using namespace cgicc;

// Define a structure to represent each team with a name and a score
struct Team {
    string name;
    int score;
};

int main() {
    try { // In an try block to handle any exceptions
        Cgicc cgi;

        // First, it get the number of teams from the form input from main.cpp
        const_form_iterator it = cgi.getElement("teamCount");
        if (it == cgi.getElements().end()) { // Print error message if the input is missing
            cout << "<h1>Error: Team count missing.</h1></body></html>";
            return 1;
        }

        int numTeams = stoi(it->getValue()); // Convert the input string to an integer
        vector<Team> teams; // Create a vector to hold all teams

        // Next, it get the team names from main.cpp and initialize scores
        for (int i = 1; i <= numTeams; ++i) {
            string key = "team" + to_string(i); // Initilaize variable 
            const_form_iterator teamIt = cgi.getElement(key); // Get input for each team
            if (teamIt != cgi.getElements().end()) { // proceed when there is an team name input
                Team team;
                team.name = teamIt->getValue(); // Assign team name
                team.score = 0;  // Initial each teams score to 0
                teams.push_back(team); // Add the team to the vector 
            } else { // Print if the team name is missing
                cout << "<h1>Error: Missing name for " << key << "</h1></body></html>";
                return 1;
            }
        }

        // Then, create JSON-formatted output of team names and scores
        string json = "[\n";
        for (size_t i = 0; i < teams.size(); ++i) { // Iterates through the teams vector to build a JSON object representing all teams and their scores.
            string cleanName = teams[i].name;
            // Escape double quotes to avoid breaking JSON format
            size_t pos = 0;
            while ((pos = cleanName.find("\"", pos)) != string::npos) {
                cleanName.replace(pos, 1, "\\\"");
                pos += 2;
            }
        
            json += "  {\n";
            json += "    \"name\": \"" + cleanName + "\",\n";
            json += "    \"score\": " + to_string(teams[i].score) + "\n";
            json += "  }";
            if (i < teams.size() - 1) json += ","; // Add comma between objects, except for the last one
            json += "\n";
        }
        json += "]"; // Close the JSON array
        
        // Then, save the JSON data to a file for future use
        ofstream outFile("/home/student/ortegbry/public_html/cgi-bin/scores.json");
        if (outFile.is_open()) { 
            outFile << json;  // Write the JSON data to file
            outFile.close(); // Close the file
        } else { // Print error message if file write fails
            cerr << "Error: Could not write to scores.json" << endl;
        }

        // Lastly, output HTTP headers to redirect the user to the game page (jeopardy.html)
        cout << "Content-type: text/html\r\n";  // HTTP header for HTML content
        cout << "Location: https://obi.kean.edu/~ortegbry@kean.edu/CPS3525/jeopardy.html\r\n\r\n"; // Redirect URL

        // Redirect to jeopardy.html after processing
        return 0;

    } catch (exception& e) {  // Catch any exceptions that occur during processing
        cout << "Content-type: text/plain\n\n"; 
        cout << "Error: " << e.what() << endl; // Display error message
        return 1;
    }
}
