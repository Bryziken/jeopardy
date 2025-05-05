#include <iostream>
#include <fstream>
#include <string>

int main() {
    std::cout << "Content-Type: application/json\n\n";
    std::ifstream in("/home/student/ortegbry/public_html/cgi-bin/scores.json");
    if (!in) {
        std::cout << "{\"error\": \"Could not open scores.json\"}";
        return 1;
    }

    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    return 0;
}
