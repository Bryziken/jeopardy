#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <cstdlib>
using namespace std;
using namespace chrono;
const string TIMER_FILE = "/home/student/ortegbry/public_html/CPS3525timer.txt"; // Update this path if needed
const int TIME_LIMIT = 30; // seconds
// Function to get current time as epoch seconds
long getCurrentTime() {
    return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}
// Function to save start time
void startTimer() {
    ofstream outFile(TIMER_FILE);
    if (outFile.is_open()) {
        long start = getCurrentTime();
        outFile << start;
        outFile.close();
    }
}
// Function to get remaining time
int getRemainingTime() {
    ofstream log("debug_timer.log", ios::app);
    log << "Trying to open timer file: " << TIMER_FILE << endl;
    log.close();
    ifstream inFile(TIMER_FILE);\
    if (!inFile.is_open()) return -1;
    long startTime;
    inFile >> startTime;
    inFile.close();
    long now = getCurrentTime();
    int elapsed = now - startTime;
    int remaining = TIME_LIMIT - elapsed;
    return remaining > 0 ? remaining : 0;
}
// Function to determine if time expired
bool isTimeUp() {
    return getRemainingTime() <= 0;
}
// CGI output
void printHeader() {
    cout << "Content-type: text/plain\n\n";
}
int main() {
    printHeader();
    // Parse QUERY_STRING to determine action
    const char* query_cstr = getenv("QUERY_STRING");
    string query = query_cstr ? string(query_cstr) : "";
    if (query == "start") {
        startTimer();
        cout << "Timer started for " << TIME_LIMIT << " seconds.\n";
    } else if (query == "check") {
        int remaining = getRemainingTime();
        if (remaining < 0) {
            cout << "Error reading timer.\n";
        } else if (remaining == 0) {
            cout << "Time's up!\n";
        } else {
            cout << "Time remaining: " << remaining << " seconds\n";
        }
    } else {
        cout << "Usage: ?start or ?check\n";
    }
    return 0;
}