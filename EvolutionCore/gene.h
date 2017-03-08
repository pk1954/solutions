// gene.h : 
//

#pragma once
#include "random.h"

class GeneTypeLimits
{
public:
    void   SetLimits( long const, long const );
    void   CheckLimits( short const ) const;
    short  ClipToLimits( short const ) const;
    double GetFactor( ) const { return m_dFactor; };
    short  GetAverageValue( ) const { return (m_sMin + m_sMax) / 2; };

private: 
	short  m_sMin;
	short  m_sMax;
    double m_dFactor;
};

class Gene
{
public:
    static int const MAX_MUTATIONRATE;
	
             Gene( )                            : m_sAllele( 0 ) { }
	explicit Gene( GeneTypeLimits const & lim ) : m_sAllele( lim.GetAverageValue( ) ) { }
    	
	void  Mutate   ( short const, GeneTypeLimits const &, Random & );
	void  SetAllele( short const sValue) { m_sAllele = sValue; };
    short GetAllele( ) const             { return m_sAllele; }

private:

	short m_sAllele;
};
