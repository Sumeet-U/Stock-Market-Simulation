#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;
// Function to parse input data
string parse_input() {
    char *data = getenv("QUERY_STRING"); // Get the query string (form data)
    if (data == NULL) {
        return "No data received!";
    }
    // Find the part after "inputData="
    string query(data);
    size_t pos = query.find("inputData=");
    if (pos != string::npos) {
        string inputData = query.substr(pos + 10); // Skip "inputData=" part
        return inputData;
    } else {
        return "Invalid data received!";
    }
}
int main() {
    // Output the HTTP header for HTML content
    cout << "Content-type: text/html\n\n";
    cout << "<html><body>";
    // Get the parsed input data
    string user_input = parse_input();
    // Display the user input as a response
    cout << "<h1>Your input:</h1>";
    cout << "<p>" << user_input << "</p>";
    cout << "</body></html>";
    return 0;
}