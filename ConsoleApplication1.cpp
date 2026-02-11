// ConsoleApplication1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cmath>
using namespace std;
int main() {
	double x = 3.893, y = 4;
	double z = hypot(x, y);
	cout << trunc(x) << endl;
	cout << trunc(y) << endl;
	cout << trunc(z) << endl;
	return 0;
}