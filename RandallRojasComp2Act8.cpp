
/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
Date: feb. 4 2026
File: RandallRojasComp2Act8.cpp
*/

/*Instructions
Write a Visual Studio program named lastnameComp2Act8.

5 pts  Write your name, email address and file name at the top of your source code in a comment.

5 pts Use cout statements to write your name, program title, and program objective to the screen so that it is the first thing seen when your program runs.This is your course header.

Write a C++ program that demonstrates the Best Practice for use of braces using conditional if, else - if, else  statements.Write a program that will help a person going on vacation to the ocean to determine whether they should to go to the beach or visit the tourist spots.

Ask them the predicted temperature.Then ask them what is the weather condition predicted to be ? Ask them to enter sunny, rainy or cloudy.

If it is predicted to be cloudy and 75 degrees or warmer, you will say “It will be cloudy, but still beach weather!”

If it is predicted to be sunny and 70 degrees or warmer, you will say “It should be a perfect day at the beach, but take a big beach umbrella!”

No matter what the temperature, if it is rainy, you will say “It will be a great day to visit the local museum!”

If none of these conditions apply, you will say, ” It might be a bit cool for the beach.Bring a sweater!”*/

#include <iostream>
#include <string>
using namespace std;
int main()
{
	// Course Header
	cout << "Name: Randall Rojas" << endl;
	cout << "Conditional else-if Statement Practice" << endl;
	cout << "Objective: Determine if the user should go to the beach or visit tourist spots based on weather conditions." << endl << endl;
	// Variables
	int temperature;
	string weatherCondition;
	// User Input
	cout << "Enter the predicted temperature (in degrees): ";
	cin >> temperature;
	cout << "Enter the predicted weather condition (sunny, rainy, or cloudy): ";
	cin >> weatherCondition;
	// Conditional Statements
	if (weatherCondition == "cloudy")
	{
		if (temperature >= 75)
		{
			cout << "It will be cloudy, but still beach weather!" << endl;
		}
		else
		{
			cout << "It might be a bit cool for the beach. Bring a sweater!" << endl;
		}
	}
	else if (weatherCondition == "sunny")
	{
		if (temperature >= 70)
		{
			cout << "It should be a perfect day at the beach, but take a big beach umbrella!" << endl;
		}
		else
		{
			cout << "It might be a bit cool for the beach. Bring a sweater!" << endl;
		}
	}
	else if (weatherCondition == "rainy")
	{
		cout << "It will be a great day to visit the local museum!" << endl;
	}
	else
	{
		cout << "It might be a bit cool for the beach. Bring a sweater!" << endl;

	}
	return 0;
}