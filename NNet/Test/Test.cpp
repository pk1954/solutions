// Test.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "MoreTypes.h"
#include "PointType.h"

using std::wcout;
using std::endl;

float const factor { 180.0f/static_cast<float>(M_PI) };

static void testVector2Radian( MicroMeter const x, MicroMeter const y )
{
    MicroMeterPnt const ump { x, y };
    Radian          const rad { Vector2Radian(ump) };
    Degrees         const deg { Radian2Degrees(rad) };
    wcout << ump << L" -> " << deg << L" Grad(" << rad << L")" << endl;
}

static void testRadian2Vector( Degrees const deg )
{
    Radian          const rad { Degrees2Radian(deg) };
    MicroMeterPnt const ump { Radian2Vector( rad ) };
    wcout << deg << L" Grad (" << rad.GetValue() << L") -> " << ump << endl;
}

int main()
{
    for ( Degrees deg = -360.0_Degrees; deg < 720.0_Degrees; deg += 15.0_Degrees )
        testRadian2Vector( deg );

    wcout << endl;

    testVector2Radian(  0.0_MicroMeter,      1.0_MicroMeter );
    testVector2Radian( -1.0_MicroMeter,      1.0_MicroMeter );
    testVector2Radian(  100.0_MicroMeter,  100.0_MicroMeter );
    testVector2Radian( -100.0_MicroMeter, -100.0_MicroMeter );
}
