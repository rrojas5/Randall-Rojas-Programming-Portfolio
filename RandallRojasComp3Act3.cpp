/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
File Name: RandallRojasComp3Act3.cpp
Date: 02/11/2026
*/

#include <iostream>
using namespace std;

int main()
{
    // Course Header
    cout << "Randall Rojas" << endl;
    cout << "Program: Multiplication Table" << endl;
    cout << "Objective: Display multiplication tables for integers 12 through 15." << endl;
    cout << endl;

    // Rows = 12 to 15
    for (int i = 12; i <= 15; i++)
    {
        // Columns = 1 to 10
        for (int j = 1; j <= 10; j++)
        {
            cout << i << " x " << j << " = " << (i * j) << endl;
        }

        cout << endl; // space between tables
    }

    return 0;
}