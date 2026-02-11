/*
Name: Randall Rojas
Email: rrojas5@cnm.edu
File: RandallRojasComp1Act9.cpp
*/

#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    // Variables
    int feet = 0;
    int inches = 0;
    int totalInches = 0;
    double decimalFeet = 0.0;

    // Course header (must be first thing seen)
    cout << "Randall Rojas" << endl;
    cout << "Randall Rojas Act9 - Height Converter" << endl;
    cout << "Objective: Convert height from feet/inches into total inches and decimal feet." << endl;
    cout << endl;

    // Input
    cout << "Enter your height in feet: ";
    cin >> feet;

    cout << "Enter your height in inches: ";
    cin >> inches;

    // Calculations
    totalInches = (feet * 12) + inches;
    decimalFeet = static_cast<double>(totalInches) / 12.0;

    // Output (2 decimal places)
    cout << fixed << setprecision(2);
    cout << feet << " feet " << inches << " inches is "
        << totalInches << " inches and that equals "
        << decimalFeet << " feet." << endl;

    return 0;
}

