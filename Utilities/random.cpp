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

// f(x) =  8x^3 - 12x^2 + 6x - 1         f(x) maps [0,1] to [-1,1]
// f(0) = -1           
// f(1) =  1

// f'(x)   = 24x^2 - 24x + 6             
// f'(0.5) = 0                          horizontal in the middle 
// f'(0)   = 6                          steep at both ends 
// f'(1)   = 6

double Random::NextWeightedDistribution( void ) 
{
	double dx   = static_cast<double>(NextRandomNumber()) / static_cast<double>(0x7fff);
    double dRes = ((4.0 * dx - 6.0) * dx + 3.0) * dx * 2.0 - 1.0;
	ASSERT_LIMITS( dRes, -1.0, 1.0 );
	return dRes;
}