/*
Name: Randall Rojas
Email: rrojas5@student.cnm.edu
File Name: RandallRojasP1.cpp
*/

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

int main()
{
    // Course header (first output)
    cout << "Name: Randall Rojas\n"
        << "Program Title: Traffic Cones - Plastic Volume Calculator\n"
        << "Program Objective: Calculate plastic volume needed (cubic inches and gallons).\n\n";

    // Constants (ALL CAPS as required)
    const double PI{ 3.14159 };
    const double BASE_SIDE{ 16.0 };
    const double BASE_THICKNESS{ 1.0 };
    const double HEIGHT{ 28.0 };
    const double OUTER_R{ 6.0 };     // 16/2 = 8, clearance 2 -> 6
    const double INNER_R{ 5.0 };     // wall thickness 1 -> 5
    const double CI_PER_GAL{ 231.0 };

    // Input
    int cones{ 0 };
    cout << "Enter the number of traffic cones to order: ";
    cin >> cones;

    // Volumes (cubic inches)
    const double baseVol{ BASE_SIDE * BASE_SIDE * BASE_THICKNESS };
    const double outerConeVol{ (PI * OUTER_R * OUTER_R * HEIGHT) / 3.0 };
    const double innerConeVol{ (PI * INNER_R * INNER_R * HEIGHT) / 3.0 };
    const double wallVol{ outerConeVol - innerConeVol };
    const double oneConeVol{ baseVol + wallVol };

    // Totals
    const double totalCI{ oneConeVol * cones };
    const double gallonsDec{ totalCI / CI_PER_GAL };
    const int gallonsInt{ static_cast<int>(ceil(gallonsDec)) };

    // Output (2 decimals)
    cout << fixed << setprecision(2);

    cout << "\n---------------- Cone Dimensions ----------------\n"
        << "Height: " << HEIGHT << " in\n"
        << "Base: " << BASE_SIDE << " in x " << BASE_SIDE << " in x " << BASE_THICKNESS << " in\n"
        << "Outer diameter: " << (OUTER_R * 2) << " in\n"
        << "Inner diameter: " << (INNER_R * 2) << " in\n"
        << "Cones ordered: " << cones << "\n\n";

    cout << "---------------- Volumes (cubic inches) ----------------\n"
        << "Base volume: " << baseVol << " in^3\n"
        << "Outer cone volume: " << outerConeVol << " in^3\n"
        << "Inner cone volume: " << innerConeVol << " in^3\n"
        << "Cone walls volume: " << wallVol << " in^3\n"
        << "One cone total volume: " << oneConeVol << " in^3\n"
        << "Total volume: " << totalCI << " in^3\n\n";

    cout << "Total gallons (decimal): " << gallonsDec << " gal\n"
        << "Gallons to order (rounded up): " << gallonsInt << " gal\n\n";

    cout << "Thank you for your order! Goodbye.\n";
    return 0;
}
