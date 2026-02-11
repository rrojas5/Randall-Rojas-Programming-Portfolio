/*
Name: Randall Rojas
Email: rrojas5@cnm.edu
File: RandallRojasComp1Act7.cpp
*/

#include <iostream>
using namespace std;
#define PI 3.14159
int main()
{
	// Course Title and Objective
    cout << "Randall Rojas" << endl;
    cout << "Temperature Conversion Program" << endl;
    cout << "This program converts Fahrenheit to Celsius." << endl;
    cout << endl;

    // Variable declarations
    double fahrenheit = 95.0;
    double celsius;
    int subtractValue = 32;
    double multiplier = 5.0 / 9.0;

    // Conversion calculation
    celsius = (fahrenheit - subtractValue) * multiplier;

    // Output result
    cout << "The temperature is " << fahrenheit
        << " degrees Fahrenheit, or "
        << celsius
        << " degrees Celsius." << endl;
    cout << "PI is approximately " << PI << endl;
    return 0;
}
