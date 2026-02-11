// RandallRojasComp1Act4.cpp : 
// Randall Rojas
// rrojas5@student.cnm.edu

// Write a program in C++ to use two constant data types.Declare and initialize two const numbers
// using uniform initialization.They should represent the value of pi and the symbol for newline.
// Use them to calculate the area of a circle and cout the calculation.The area of a circle is pi* 
// radius* radius.The radius is 5.

#include <iostream>
using namespace std;

int main()
{
    const double pi{ 3.141592653589793 };
    const char newline{ '\n' };
    const int radius{ 5 };

    double area{ pi * radius * radius };

    cout << "The area of the circle is: " << area << newline;

    return 0;
}

