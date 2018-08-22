// random.cpp :
//

#include "stdafx.h"
#include <cmath> 
#include "debug.h"
#include "random.h"

void Random::InitializeRandom( )
{
    m_ulRandomSeed = 0L;
    m_bValid       = false;
    m_dX2          = 0.0f;
}

unsigned int Random::NextRandomNumber( void )  // delivers 15 bit pseudo random number
{
    return( ((m_ulRandomSeed = m_ulRandomSeed * 214013L + 2531011L) >> 16) & 0x7fff );
}

bool Random::NextBooleanValue( void )
{
    return NextRandomNumber( ) > 0x7fff / 2;
}

double Random::NextWeightedDistribution( void ) 
{
	double dRand = static_cast<double>(NextRandomNumber()) / static_cast<double>(0x7fff);
    double dRes  = ((4.0 * dRand - 6.0) * dRand + 3.0) * dRand * 2.0 - 1.0;
	ASSERT_LIMITS( dRes, -1.0, 1.0 );
	return dRes;
}