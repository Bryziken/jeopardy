#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/FormFile.h>
#include <cgicc/FormEntry.h>
#include <algorithm>

using namespace std;
using namespace cgicc;

int validate() {
    try {
        Cgicc cgi;

        cout << HTTPHTMLHeader() << endl;
        cout << "<html><head><title>Upload Successful</title></head><body style='background:#001f3f; color:gold; text-align:center; font-family:sans-serif;'>\n";

        // Get the uploaded file
        const_file_iterator file = cgi.getFile("jsonFile");
        if (file == cgi.getFiles().end()) {
            cout << "<h1>Error: JSON file not received.</h1></body></html>";
            return 1;
        }

        // Save uploaded JSON file
        ofstream out("/home/student/ortegbry/public_html/cgi-bin/questions.json");
        if (!out) {
            cout << "<h1>Error: Could not write file. Check permissions.</h1></body></html>";
            return 1;
        }

        file->writeToStream(out);
        out.close();

        // Get team count
        const_form_iterator it = cgi.getElement("teamCount");
        if (it == cgi.getElements().end()) {
            cout << "<h1>Error: Team count missing.</h1></body></html>";
            return 1;
        }

        int numTeams = stoi(it->getValue());
        vector<string> teamNames;

        for (int i = 1; i <= numTeams; ++i) {
            string key = "team" + to_string(i);
            const_form_iterator teamIt = cgi.getElement(key);
            if (teamIt != cgi.getElements().end()) {
                teamNames.push_back(teamIt->getValue());
            } else {
                cout << "<h1>Error: Missing name for " << key << "</h1></body></html>";
                return 1;
            }
        }

        // Auto-submit hidden form to scores.cpp
        cout << "<h1>Upload successful!</h1>\n";
        cout << "<p>Redirecting to game board...</p>\n";

        cout << "<form id='redirectForm' action='../cgi-bin/scores' method='post'>\n";
        cout << "<input type='hidden' name='teamCount' value='" << numTeams << "'>\n";
        for (int i = 0; i < numTeams; ++i) {
            cout << "<input type='hidden' name='team" << (i + 1) << "' value='" << teamNames[i] << "'>\n";
        }
        cout << "</form>\n";

        // Auto-submit with JavaScript
        cout << "<script>document.getElementById('redirectForm').submit();</script>\n";
        cout << "</body></html>\n";

    } catch (exception& e) {
        cout << "Content-type: text/plain\n\n";
        cout << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
