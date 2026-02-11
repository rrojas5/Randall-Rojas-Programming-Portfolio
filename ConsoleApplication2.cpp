/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
File: RojasComp2Act10.cpp
*/

#include <iostream>
#include <cmath>      // needed for pow()
using namespace std;

int main()
{
    // Program header
    cout << "\nFind the Volume of a Pyramid\n\n";

    // Declare variables
    double length{ 0.0 }, height{ 0.0 };
    char comma{ ',' };
    double volPyramid{ 0.0 };

    // Ask the user for the dimensions
    cout << "\nPlease enter the length and height of your pyramid, with a comma between them (ex 5,8)\n"
        << "        =>  ";

    // Correct input
    cin >> length >> comma >> height;

    // Validate comma
    if (comma != ',')
    {
        cout << "\nInput error: Please separate numbers with a comma like 5,8\n\n";
        return 0;
    }

    // Calculate the volume
    // Volume = base * base * height / 3
    volPyramid = (pow(length, 2) * height) / 3.0;

    // Report to the user
    cout << "\nBased on the dimensions you gave this program:\n "
        << "length: " << length << "   height: " << height << endl
        << "\nThe volume of your pyramid is: " << volPyramid
        << " cubic feet.\n\n";

    return 0;
}
