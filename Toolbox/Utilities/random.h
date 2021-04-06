// random.h : 
//

#pragma once

#include "debug.h"

class Random
{
public:
	Random()
	{
		Initialize();
	}

    virtual ~Random() {};

	static unsigned int const MAX_VAL = 0x7fff;
	
	void Initialize()
	{
		m_ulRandomSeed = 0L;
		m_bValid       = false;
		m_dX2          = 0.0f;
	}

	unsigned int NextRandomNumber( void )  // delivers 15 bit pseudo random number
	{
		return( ((m_ulRandomSeed = m_ulRandomSeed * 214013L + 2531011L) >> 16) & MAX_VAL );
	}

	bool NextBooleanValue( void )
	{
		return NextRandomNumber() > (MAX_VAL / 2);
	}

	unsigned int NextRandomNumberScaledTo( unsigned int const uiFactor )
	{
		unsigned int const uiRandom  = NextRandomNumber();
		unsigned int const uiProduct = uiRandom * uiFactor;
		unsigned int const uiResult  = uiProduct >> 15;
		return uiResult;
	}

	// f(x) =  8x^3 - 12x^2 + 6x - 1         f(x) maps [0,1] to [-1,1]
	// f(0) = -1           
	// f(1) =  1

	// f'(x)   = 24x^2 - 24x + 6             
	// f'(0.5) = 0                          horizontal in the middle 
	// f'(0)   = 6                          steep at both ends 
	// f'(1)   = 6

	double NextWeightedDistribution( void ) 
	{
		double dx   = static_cast<double>(NextRandomNumber()) / static_cast<double>(MAX_VAL);
		double dRes = ((4.0 * dx - 6.0) * dx + 3.0) * dx * 2.0 - 1.0;
		AssertLimits( dRes, -1.0, 1.0 );
		return dRes;
	}

private:

    unsigned long m_ulRandomSeed;    // random generator
    bool          m_bValid;          // random generator for standard distribution
    double        m_dX2;             // random generator for standard distribution
};
