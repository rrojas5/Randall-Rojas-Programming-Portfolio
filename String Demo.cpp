// String Demo.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
using namespace std;
int main()
{
	string str1 = "Hello, ";
	string str2 = "World!";
	string str3;
	// Concatenation
	str3 = str1 + str2;
	cout << "Concatenated String: " << str3 << endl;
	// Length
	cout << "Length of str3: " << str3.length() << endl;
	// Substring
	string subStr = str3.substr(7, 5);
	cout << "Substring (7,5): " << subStr << endl;
	// Find
	size_t pos = str3.find("World");
	if (pos != string::npos) {
		cout << "'World' found at position: " << pos << endl;
	} else {
		cout << "'World' not found" << endl;
	}
	// Replace
	str3.replace(7, 5, "C++");
	cout << "After Replacement: " << str3 << endl;
	return 0;
}