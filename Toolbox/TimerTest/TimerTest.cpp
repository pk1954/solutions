// TimerTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "windows.h"
#include <math.h>
#include <random>
#include <iostream>

extern double GetNormalDistribution( );

using namespace std;

#define TYPE double

int _tmain(int argc, _TCHAR* argv[])
{
//	const int COUNT = 1000000000L;
	const int COUNT = 10000000L;

	DWORD milliSeconds = GetTickCount();
/*
	TYPE l1 = 1;
	TYPE l2 = 1;
	TYPE l3 = 1;

	for ( int i = 0; i < COUNT; i++ )
	{
		l3 = l1 + l2;
		l1 = (l3 + l2) / 2;
	}
*/

long m_lMax = 1000;
long m_lMin = 0;
long m_lAllele = 123;

double dFactor = pow(2, -12);
static const long MAX_MUTATIONRATE = 100;

	for ( int i = 0; i < COUNT; i++ )
	{
/*
        int iMutationRate = 50;
        int iRand   = rand();                         //        0 <= iRand  <= 32767
        int iRand2  = iRand - (RAND_MAX/2);                            // -16383   <= iRand2 <= 16384
        int iRand3  = iMutationRate * iRand2;                          // -1638300 <= iRand3 <= 1638400
        int iRate   = (iRand3 + (RAND_MAX/4)) / (RAND_MAX/2);          // add half divisor for rounding instead of cut off
        long lDelta    = ( iRate > 0 ) ? (m_lMax - m_lAllele) : (m_lAllele - m_lMin);             
        long lMaxDelta = m_lMax - m_lMin;
        long lGrowth = lDelta;
        lGrowth = (lGrowth * lDelta + lMaxDelta        / 2) / lMaxDelta;
        lGrowth = (lGrowth * iRate  + MAX_MUTATIONRATE / 2) / MAX_MUTATIONRATE;
        m_lAllele += lGrowth;
        double dRand = (double)iRand2 * dFactor;
        double dExp  = - (dRand * dRand);
        double dGauss = exp( dExp );
        double dRes = dGauss;
*/
        double dNumber = GetNormalDistribution();
        double dRes = dNumber; 
	}

	milliSeconds = GetTickCount() - milliSeconds;

   	cout << milliSeconds << endl;

	return 0;
}
