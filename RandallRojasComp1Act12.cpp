/*
Name: Randall Rojas
Email: rrojas5@cnm.edu
File: lastnameComp1Act12.cpp
*/

#include <iostream>
#include <string>
using namespace std;

int main()
{
    // Course Header (must be first output)
    cout << "Randall Rojas" << endl;
    cout << "lastnameComp1Act12 - String Manipulation" << endl;
    cout << "Objective: Demonstrate manipulation of a C++ string using built-in string functions." << endl;
    cout << endl;

    // Create string variable
    string message = "Welcome, student!";

    // Output original string
    cout << "Original string: " << message << endl;

    // Output length of the string
    cout << "Length of string: " << message.length() << endl;

    // Output the character at index 1
    cout << "Character at index 1: " << message.at(1) << endl;

    // Check if string is empty
    cout << "Is the string empty? ";
    if (message.empty())
        cout << "Yes" << endl;
    else
        cout << "No" << endl;

    // Append "end" to the string
    message.append("end");
    cout << "After appending \"end\": " << message << endl;

    // Output new length
    cout << "New length of string: " << message.length() << endl;

    return 0;
}
