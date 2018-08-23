// gene.h : 
//

#pragma once

#include "random.h"
#include "geneTypeLimits.h"

class Gene
{
public:
    static int const MAX_MUTATIONRATE;
	
    Gene( )
		: m_sAllele( 0 ) 
	{ }

	explicit Gene( GeneTypeLimits const & lim ) 
		: m_sAllele( lim.GetAverageValue( ) ) 
	{ }
    	
	void  Mutate( short const, GeneTypeLimits const &, Random & );

	void SetAllele( short const sValue) 
	{ 
		m_sAllele = sValue; 
	};

    short GetAllele( ) const
	{ 
		return m_sAllele; 
	}

private:

	short m_sAllele;
};
