#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

// Helper to get POST data from stdin
string getPostData() {
    char* lenStr = getenv("CONTENT_LENGTH");
    if (!lenStr) return "";
    int len = atoi(lenStr);
    string data(len, '\0');
    cin.read(&data[0], len);
    return data;
}

// Simple URL decoder
string urlDecode(const string& str) {
    string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            string hex = str.substr(i + 1, 2);
            result += static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

// Parse key=value pairs from POST data
string getValue(const string& data, const string& key) {
    size_t pos = data.find(key + "=");
    if (pos == string::npos) return "";
    size_t start = pos + key.length() + 1;
    size_t end = data.find("&", start);
    string value = data.substr(start, end - start);
    return urlDecode(value);
}

int main() {
    cout << "Content-type: text/html\r\n\r\n";

    string postData = getPostData();
    string numStr = getValue(postData, "numTeams");

    int numTeams;
    stringstream(numStr) >> numTeams;

    cout << R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Enter Team Names</title>
  <style>
    body {
      background-color: #001f3f;
      color: gold;
      font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
      text-align: center;
      padding: 50px;
    }

    h1 {
      font-size: 3rem;
      text-shadow: 2px 2px #000;
    }

    form {
      margin-top: 30px;
    }

    input[type='text'] {
      font-size: 1rem;
      margin: 5px 0;
      padding: 8px;
      border-radius: 4px;
      border: 1px solid #ccc;
      width: 200px;
    }

    input[type='submit'] {
      margin-top: 20px;
      font-size: 1.2rem;
      padding: 8px 16px;
      background-color: #2ECC40;
      color: white;
      border: none;
      border-radius: 8px;
      cursor: pointer;
    }
  </style>
</head>
<body>
)";

    if (numTeams < 2 || numTeams > 9) {
        cout << "<h1>Error: Please enter a number between 2 and 9.</h1>";
    } else {
        cout << "<h1>Enter Team Names</h1>";
        cout << "<form method='post' action='index.cgi'>\n";
        for (int i = 1; i <= numTeams; ++i) {
            cout << "<input type='text' name='team" << i
                 << "' placeholder='Team " << i << " Name'><br>\n";
        }
        cout << "<input type='submit' value='Start Game'>\n";
        cout << "</form>\n";
    }

    cout << "</body></html>\n";

    return 0;
}
