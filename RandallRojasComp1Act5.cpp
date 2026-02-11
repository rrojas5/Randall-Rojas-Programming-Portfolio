/*
Name: Randall Rojas
Email: rrojas5@cnm.edu
File: RandallRojasComp1Act5.cpp
*/

#include <iostream>
using namespace std;

int main()
{
    // Course Header
    cout << "Randall Rojas" << endl;
    cout << "Arithmetic Calculations Program" << endl;
    cout << "This program performs and displays the results of the calculations." << endl;
    cout << endl;

    // Calculations
    int result1 = (5 + 2) * 4;
    int result2 = 10 / (5 - 3);
    int result3 = 8 + (12 * (6 - 2));

    // Display results
    cout << "Result of (5 + 2) * 4 = " << result1 << endl;
    cout << "Result of 10 / (5 - 3) = " << result2 << endl;
    cout << "Result of 8 + (12 * (6 - 2)) = " << result3 << endl;

    return 0;
}

/* Instructions
Write a Visual Studio program named lastnameComp1Act5.Write your name, email address and file name at the top of your source code in a comment.Use cout
statements to write your name, program title, and program objective to the screen so
that it is the first thing seen when your program runs.  This is your course header.
Write a program in C++ to print the result of these calculations:
Add 5 and 2 and then multiply the sum by 4.Divide 10 by the difference between 5 and 3.Add 8 to the
result of multiplying 12 by the difference between 6 and 2.Create variables to hold the results of
the computations and display each result.Delete the intermediate files, zip your project and upload
it to this activity in Brightspace.*/