// gene.cpp : 
//


#include "stdafx.h"
#include "assert.h"
#include "limits.h"
#include "gene.h"
#include "random.h"

//////////// GeneTypeLimits ////////////////////
 
void GeneTypeLimits::SetLimits( long const lLo, long const lHi )
{
    assert( lLo >= SHRT_MIN );
    assert( lLo <= SHRT_MAX );
    short const sLo = static_cast<short>( lLo );
    assert( lHi >= SHRT_MIN );
    assert( lHi <= SHRT_MAX );
    short const sHi = static_cast<short>( lHi );

    m_sMin    = sLo;
    m_sMax    = ( sHi >= sLo ) ? sHi : sLo;
    m_dFactor = static_cast<double>(m_sMax - m_sMin) / Gene::MAX_MUTATIONRATE;
}

short GeneTypeLimits::ClipToLimits( short const sAllele ) const
{
    if ( sAllele > m_sMax ) 
        return m_sMax;
    else if ( sAllele < m_sMin ) 
        return m_sMin;
    else 
        return sAllele;
}

void GeneTypeLimits::CheckLimits( short const sValue ) const 
{
    assert( sValue >= m_sMin );
    assert( sValue <= m_sMax );
};

//////////// Gene ////////////////////

int const Gene::MAX_MUTATIONRATE = 100;

void Gene::Mutate( short const sMutationRate, GeneTypeLimits const & lim, Random & random )
{
    assert( sMutationRate <= MAX_MUTATIONRATE );

    double const dRand = random.NextNormalDistribution() * lim.GetFactor( );

    assert( dRand * static_cast<double>(sMutationRate) < static_cast<double>(INT_MAX) );
    assert( dRand * static_cast<double>(sMutationRate) > static_cast<double>(INT_MIN) );

    int const iMutation = static_cast<int>(dRand) * sMutationRate;
    m_sAllele = lim.ClipToLimits( m_sAllele + static_cast<short>(iMutation) );
}
    
