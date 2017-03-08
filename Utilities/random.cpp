// random.cpp :
//

#include "stdafx.h"
#include <cmath> 
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

double Random::NextNormalDistribution( void )
{
    double dRes;

    if ( m_bValid )
    {	// return stored value
        dRes = m_dX2;
        m_bValid = false;
    }
    else
    {	// generate two values, store one, return one
        double dV1, dV2, dSx;
        do
        {
            static double const dMax    = 32767.0;
            static double const dFactor = 2.0 / dMax;
            dV1 = dFactor * static_cast<double>(NextRandomNumber()) - 1.0;
            dV2 = dFactor * static_cast<double>(NextRandomNumber()) - 1.0;
            dSx = dV1 * dV1 + dV2 * dV2;
        } while (dSx >= 1.0); 

        double const dFx = sqrt(-2.0 * log(dSx) / dSx);
        m_dX2 = dFx * dV2;   // save second value for next call
        dRes  = dFx * dV1;
        m_bValid = true;
    }

    return dRes;
}

