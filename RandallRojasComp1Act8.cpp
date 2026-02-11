/* Randall Rojas
   rrojas5@cnm.edu
   RandallRojasComp1Act8.ccp
   */
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
// Course Header
	cout << "Randall Rojas\n";
	cout << "-----------------\n";
	cout << "I/O Manipulators program\n";
	cout << "This program demonstrates the use of I/O manipulators.\n\n";
	cout << endl;
// Variables
	float number1{ 2.558 };
	float number2{ 9.3456};
// Calculation
	float result = number1 * number2;
// Result Without formatting
	cout << "Result without formatting: " << result << endl;
	cout << "----------------------------------------" << endl;
// Result With formatting
	cout << fixed << setprecision(2);
	cout << "Result with formatting: " << result << endl;
	cout << "----------------------------------------" << endl;
// Display with fixed and setprecision(4)
	cout << fixed << setprecision(4);
	cout << "Number 1 with 4 decimal places: " << number1 << endl;
	cout << "----------------------------------------" << endl;
	cout << "Number 2 with 4 decimal places: " << number2 << endl;
	cout << "----------------------------------------" << endl;
// Reset to default formatting
	cout.setf(ios::fixed | ios::showpoint);
	cout.precision(4);
	cout << "Result with 4 decimal places: " << result << endl;
// End program
	return 0;
}