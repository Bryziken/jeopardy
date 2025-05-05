#include <iostream>
#include <fstream>
#include <string>
int main() {
    std::cout << "Content-Type: application/json\n\n";
    std::ifstream in("questions.json");
    if (!in) {
        std::cout << "{\"error\": \"Could not open questions.json\"}";
        return 1;
    }
    std::string line;
    while (std::getline(in, line)) {
        std::cout << line << "\n";
    }
    return 0;
}