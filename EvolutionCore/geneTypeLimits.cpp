// geneTypeLimits.cpp : 
//

#include "stdafx.h"
#include <algorithm>
#include "util.h"
#include "gene.h"
#include "geneTypeLimits.h"

void GeneTypeLimits::SetLimits( long const lLo, long const lHi )
{
    short const sLo = CastToShort( lLo );
    short const sHi = CastToShort( lHi );

    m_sMin    = sLo;
    m_sMax    = max( sHi, sLo );
    m_dFactor = (static_cast<double>(m_sMax - m_sMin)) / Gene::MAX_MUTATIONRATE;
}

short GeneTypeLimits::ClipToLimits( short const sAllele ) const
{
	return ClipToMinMax( sAllele, m_sMin, m_sMax );
}
    