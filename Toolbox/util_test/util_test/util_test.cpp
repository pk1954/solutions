// util_test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "MoreTypes.h"
#include "PointType.h"
#include "LineType.h"

using MicroMeterLine = LineType< MicroMeter >;

MicroMeter um0 =   0.0_MicroMeter; 
MicroMeter um1 = 100.0_MicroMeter; 

int main()
{

    MicroMeterPoint p0 { um0, um0 };
    MicroMeterPoint p1 { um1, um0 };
    MicroMeterPoint p2 { um0, um1 };
    MicroMeterPoint p3 { um1, um1 };
    MicroMeterPoint u0 {  50.0_MicroMeter, -50.0_MicroMeter };
    MicroMeterPoint u1 {  50.0_MicroMeter,  50.0_MicroMeter };

    MicroMeterLine l1 { p0, p1 };
    MicroMeterLine l2 { u0, u1 };

    bool bRes { Intersect( l1, l2 ) }; assert( bRes );

    std::cout << "Hello World!\n" << bRes;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
