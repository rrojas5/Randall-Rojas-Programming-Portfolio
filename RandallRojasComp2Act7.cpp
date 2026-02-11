/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
Date: feb, 4 2026*/

/*Instructions
Write a Visual Studio program named lastnameComp2Act7.

5 pts  Write your name, email address and file name at the top of your source code in a comment.

5 pts Use cout statements to write your name, program title, and program objective to the screen so that it is the first thing seen when your program runs.This is your course header.

Write a C++ program that uses the  ternary operator to write this if - else condition conditional : If more than 12 students sign up for C++ I class, if will  be taught.If not, the
class will be cancelled.Declare that variable for the number of students registered for a C++ I class.Ask the user for the number of students registered and display the result of the conditional.*/

#include <iostream>
using namespace std;

int main() {
	// Course Header
	cout << "Name: Randall Rojas" << endl;
	cout << "Program Title: RandallRojasComp2Act7" << endl;
	cout << "Program Objective: Determine if C++ I class will be taught or cancelled based on student registration." << endl;

	// Prompt user for number of students
    int numStudents;
    cout << "Enter the number of students registered for C++ I: ";
    cin >> numStudents;


	// Ternary operator to determine if class will be taught or cancelled
    string result = (numStudents > 12) ? "The class will be taught." : "The class will be cancelled.";
    cout << result << endl;

    return 0;
}