// geneTypeLimits.h : 
//

#pragma once

#include "debug.h"

class GeneTypeLimits
{
public:
    void  SetLimits( long const, long const );
    short ClipToLimits( short const ) const;
    
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
