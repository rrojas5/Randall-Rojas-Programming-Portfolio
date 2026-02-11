/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
File Name: RandallRojasComp3Act4.cpp
Date: 02/11/2026
*/

#include <iostream>
using namespace std;

int main()
{
    // Course Header
    cout << "Randall Rojas" << endl;
    cout << "Program: Prime Number Checker" << endl;
    cout << "Objective: Determine whether a number is prime using loops." << endl << endl;

    int number;
    char again;

    do
    {
        cout << "Enter an integer: ";
        cin >> number;

        bool isPrime = (number >= 2);  

        // Prime check loop
        for (int n = 2; n < number && isPrime; n++)
        {
            if (number % n == 0)
                isPrime = false;
        }

        // Result output
        cout << number
            << (isPrime ? " is PRIME." : " is NOT prime.")
            << endl;

        cout << "Check another number? (y/n): ";
        cin >> again;
        cout << endl;

    } while (again == 'y' || again == 'Y');

    cout << "Goodbye!" << endl;

    return 0;
}