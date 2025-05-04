#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
int main() {
    std::cout << "Content-type: text/html\n\n";
    const char* contentType = std::getenv("CONTENT_TYPE");
    if (!contentType || std::string(contentType).find("multipart/form-data") == std::string::npos) {
        std::cout << "<h1>Error: Expected multipart/form-data</h1>";
        return 1;
    }
    // Read all input data into a string
    std::string data;
    char ch;
    while (std::cin.get(ch)) {
        data += ch;
    }
    // Locate the JSON part in the multipart data
    size_t start = data.find("Content-Type: application/json");
    if (start == std::string::npos) {
        std::cout << "<h1>Error: JSON part not found.</h1>";
        return 1;
    }
    size_t jsonStart = data.find("{", start);
    size_t jsonEnd = data.rfind("}");
    if (jsonStart == std::string::npos || jsonEnd == std::string::npos || jsonEnd <= jsonStart) {
        std::cout << "<h1>Error: Invalid JSON format or no JSON uploaded.</h1>";
        return 1;
    }
    // Extract JSON content
    std::string jsonContent = data.substr(jsonStart, jsonEnd - jsonStart + 1);
    // Write to file
    std::ofstream out("/home/student/ortegbry/public_html/cgi-bin/questions.json");
    if (!out) {
        std::cout << "<h1>Error writing file. Check permissions.</h1>";
        return 1;
    }
    out << jsonContent;
    out.close();
    std::cout << "<html><head>";
    std::cout << "<meta http-equiv=\"refresh\" content=\"2;url=../CPS3525/jeopardy.html\">";
    std::cout << "<title>Upload Successful</title></head><body style='color:gold; background:#001f3f; text-align:center; font-family:sans-serif;'>";
    std::cout << "<h1>Upload successful!</h1>";
    std::cout << "<p>You will be redirected to the game board shortly...</p>";
    std::cout << "</body></html>";
    return 0;
}