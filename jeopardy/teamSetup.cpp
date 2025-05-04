#include <iostream>
#include <regex>
#include <cgicc/Cgicc.h>
#include <cgicc/FormEntry.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
using namespace std;
using namespace cgicc;
int main() {
    try {
        Cgicc cgi;
        cout << HTTPHTMLHeader() << endl;
        cout << html() << endl;
        cout << head(title("Team Setup")) << endl;
        cout << "<style>"
            << "body { background-color: #001f3f; color: gold; font-family: sans-serif; text-align: center; padding: 50px; }"
            << "input { font-size: 1.2rem; margin: 10px; padding: 8px; border-radius: 5px; border: none; width: 250px; }"
            << "button { font-size: 1.2rem; padding: 10px 20px; background-color: #0074D9; color: white; border: none; border-radius: 8px; cursor: pointer; }"
            << "button:hover { background-color: #39CCCC; }"
            << "</style>" << endl;
        cout << body() << endl;
        const_form_iterator it = cgi.getElement("numTeams");
        if (it == cgi.getElements().end()) {
            cout << h2("Missing number of teams.") << endl;
            cout << "</body></html>";
            return 1;
        }
        string numTeamsStr = it->getValue();
        if(numTeamsStr.length() != 1) {
            cout << h2("Invalid input: Please enter a number between 2 and 9.") << endl;
            cout << "</body></html>";
            return 1;
        }
        if(numTeamsStr[0] < 50 || numTeamsStr[0] > 57) {
            cout << h2("Invalid input: Please enter a number between 2 and 9.") << endl;
            cout << "</body></html>";
            return 1;
        }
        int numTeams = stoi(numTeamsStr);
        if (numTeams < 2 || numTeams > 9) {
            cout << h2("Number of teams must be between 2 and 9.") << endl;
            cout << "</body></html>";
            return 1;
        }
        // Form to enter team names
        cout << h1("Enter Team Names") << endl;
        cout << "<form action='../cgi-bin/uploadQuestions' method='post' enctype='multipart/form-data'>" << endl;
        for (int i = 1; i <= numTeams; ++i) {
            cout << "<input type='text' name='team" << i << "' placeholder='Team " << i << " Name' required><br>" << endl;
        }
        cout << "<input type='file' name='jsonFile' accept='.json' required><br><br>" << endl;
        cout << "<input type='hidden' name='teamCount' value='" << numTeams << "'>" << endl;
        cout << "<button type='submit'>Continue to Game</button>" << endl;
        cout << "</form>" << endl;
        cout << "</body></html>" << endl;
    } catch (exception& e) {
        cout << "Content-type: text/plain\n\n";
        cout << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}