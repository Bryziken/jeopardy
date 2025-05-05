#include <iostream>     // For input/output (e.g., cout)
#include <fstream>      // For file input/output (ofstream, ifstream)
#include <chrono>       // For handling system time
#include <ctime>        // For converting time if needed
#include <string>       // For string manipulation
#include <cstdlib>      // For getenv()

using namespace std;
using namespace chrono;

const int TIME_LIMIT = 30; // The countdown timer limit in seconds

// Function to get the current system time in seconds since the epoch (UNIX timestamp)
long getCurrentTime() {
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

// Function to extract a parameter value from the CGI query string
// For example, in "start&id=5", getParam("start&id=5", "id") will return "5"
string getParam(const string& query, const string& key) {
    size_t pos = query.find(key + "=");               // Find position of key
    if (pos == string::npos) return "";               // If key not found, return empty string
    size_t start = pos + key.length() + 1;            // Start index of the value
    size_t end = query.find("&", start);              // Find next & symbol
    if (end == string::npos) end = query.length();    // If no &, go to end of query string
    return query.substr(start, end - start);          // Extract and return the value
}

// Function to generate a file path for storing the timer value for a specific question ID
string getTimerFilePath(const string& id) {
    return "timer_" + id + ".txt";
}

// Function to print the CGI header (required for web output)
void printHeader() {
    cout << "Content-type: text/plain\n\n"; // Tells the browser that the content is plain text
}

int main() {
    // Print the CGI header so the browser knows how to render the response
    printHeader();

    // Get the QUERY_STRING environment variable (contains parameters like ?start&id=3)
    const char* query_cstr = getenv("QUERY_STRING");
    // Convert it to a C++ string (if it exists), otherwise use an empty string
    string query = query_cstr ? string(query_cstr) : "";

    // Determine the action: if "start" param exists, action is "start"; otherwise "check"
    string action = getParam(query, "start").empty() ? "check" : "start";

    // Extract the question ID from the query string
    string questionId = getParam(query, "id");

    // If no question ID was provided, print error and exit
    if (questionId.empty()) {
        cout << "Error: Missing question ID.\n";
        return 0;
    }

    // Generate the file path for the current question's timer file
    string filePath = getTimerFilePath(questionId);

    // ----------------------------
    // START ACTION: Start a timer
    // ----------------------------
    if (action == "start") {
        // Open file for writing (this will create or overwrite the timer file)
        ofstream outFile(filePath);
        if (!outFile) {
            cout << "Error: Cannot write to " << filePath << endl;
            return 0;
        }

        // Write the current time (timestamp) to the file
        outFile << getCurrentTime();
        outFile.close(); // Close the file

        // Notify that the timer has started
        cout << "Timer started for " << TIME_LIMIT << " seconds.\n";
    }

    // ----------------------------
    // CHECK ACTION: Check timer status
    // ----------------------------
    else if (action == "check") {
        // Open the timer file to read the start time
        ifstream inFile(filePath);
        if (!inFile) {
            cout << "Error reading timer file: " << filePath << endl;
            return 0;
        }

        long startTime;     // Variable to hold the recorded start time
        inFile >> startTime; // Read the start time from the file
        inFile.close();     // Close the file

        long now = getCurrentTime();        // Get current time
        int elapsed = now - startTime;      // Calculate how much time has passed
        int remaining = TIME_LIMIT - elapsed; // Calculate remaining time

        // Output time remaining or time's up
        if (remaining <= 0)
            cout << "Time's up!";
        else
            cout << "Time remaining: " << remaining << " seconds";
    }

    // ----------------------------
    // INVALID ACTION: Neither start nor check
    // ----------------------------
    else {
        // Print usage instructions
        cout << "Usage: ?start&id=QUESTIONID or ?check&id=QUESTIONID";
    }

    return 0; // End of program
}