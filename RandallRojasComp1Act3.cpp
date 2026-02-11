/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
File Name: RandallRojasComp1Act3.cpp
*/

#include <iostream>
using namespace std;

int main()
{
    // Course header output
    cout << "Name: Randall Rojas" << endl;
    cout << "Program Title: Students and Visitors Sum" << endl;
    cout << "Program Objective: Calculate and display the total number of people in a class." << endl;
    cout << endl;

    // Variable declarations using uniform initialization
    int students{ 28 };
    int visitors{ 3 };

    // Calculate the sum
    int totalPeople{ students + visitors };

    // Output the result
    cout << "Total number of people in the class: " << totalPeople << endl;

    return 0;
}
