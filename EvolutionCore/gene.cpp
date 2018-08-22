// gene.cpp : 
//


#include "stdafx.h"
#include "assert.h"
#include "util.h"
#include "debug.h"
#include "limits.h"
#include "gene.h"
#include "random.h"

//////////// GeneTypeLimits ////////////////////
 
void GeneTypeLimits::SetLimits( long const lLo, long const lHi )
{
    short const sLo = CastToShort( lLo );
    short const sHi = CastToShort( lHi );

    m_sMin    = sLo;
    m_sMax    = ( sHi >= sLo ) ? sHi : sLo;
    m_dFactor = (static_cast<double>(m_sMax - m_sMin)) / Gene::MAX_MUTATIONRATE;
}

short GeneTypeLimits::ClipToLimits( short const sAllele ) const
{
	return ClipToMinMax( sAllele, m_sMin, m_sMax );
}

//////////// Gene ////////////////////

int const Gene::MAX_MUTATIONRATE = 100;

void Gene::Mutate( short const sMutationRate, GeneTypeLimits const & lim, Random & random )
{
    assert( sMutationRate <= MAX_MUTATIONRATE );

    double const dRand = random.NextWeightedDistribution() * lim.GetFactor( );

    m_sAllele = lim.ClipToLimits( CastToInt( m_sAllele + dRand * sMutationRate ) );
}
    