// gene.cpp : 
//

#include "stdafx.h"
#include "util.h"
#include "gene.h"
#include "random.h"

int const Gene::MAX_MUTATIONRATE = 100;

void Gene::Mutate( short const sMutationRate, GeneTypeLimits const & lim, Random & random )
{
    assert( sMutationRate <= MAX_MUTATIONRATE );

    double const dRand = random.NextWeightedDistribution() * lim.GetFactor( );

    m_sAllele = lim.ClipToLimits( CastToInt( m_sAllele + dRand * sMutationRate ) );
}
    