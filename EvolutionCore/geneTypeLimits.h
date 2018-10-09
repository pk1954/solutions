// geneTypeLimits.h : 
//

#pragma once

#include "debug.h"
#include "util.h"

class GeneTypeLimits
{
public:
	void SetLimits( long const lLo, long const lHi )
	{
		short const sLo = CastToShort( lLo );
		short const sHi = CastToShort( lHi );

		m_sMin    = sLo;
		m_sMax    = max( sHi, sLo );
		m_dFactor = (static_cast<double>(m_sMax - m_sMin)) / 100;  // percentage
	}

	short ClipToLimits( short const sAllele ) const
	{
		return ClipToMinMax( sAllele, m_sMin, m_sMax );
	}
    
	double GetFactor( ) const 
	{ 
		return m_dFactor; 
	};

	short  GetAverageValue( ) const 
	{ 
		return (m_sMin + m_sMax) / 2; 
	};
	
	void CheckLimits( short const sValue ) const 
	{
		ASSERT_LIMITS( sValue, m_sMin, m_sMax );
	};

private: 
	short  m_sMin;
	short  m_sMax;
    double m_dFactor;
};
