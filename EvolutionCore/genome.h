// genome.h : 
//

#pragma once

#include <array>
#include "gene.h"
#include "config.h"
#include "EnumArray.h"
#include "ActionOptions.h"
#include "EvolutionTypes.h"

class Random;

//lint -sem(Genome::init,initializer)

class Genome
{
public:
    
    Genome( );
    void       InitGenome( );
    Action::Id GetOption( bool const, bool const, ENERGY_UNITS const, EVO_GENERATION const, Random & ) const;
    void       Mutate( PERCENT const, Random & );
    void       Recombine( Genome const &, Genome const &, Random & );

    static void InitClass( );

    // for statistics

    short GetAllele( GeneType::Id const geneType ) const 
	{ 
		return m_aGene[geneType].GetAllele(); 
	};

	static bool IsEnabled( Action::Id const action ) 
	{ 
		return m_abActionEnabled[action];; 
    };

private:
 
    EnumArray< Gene, GeneType > m_aGene;  
                                                                   
	void setGene( GeneType::Id const type, short const sValue )
	{
		m_aLimits[ type ].CheckLimits( sValue );
		m_aGene  [ type ].SetAllele( sValue );
	}

	// static members and functions

	static unsigned int const MAX_LIFE_SPAN = 200;
	static std::array< unsigned int, MAX_LIFE_SPAN + 1 > m_mortalityTable;

	static EnumArray< bool,  Action  > m_abActionEnabled;
    static EnumArray< GeneTypeLimits, GeneType > m_aLimits;
    static Genome        m_genomeTemplate;
	static ActionOptions m_options;

    static void setLimits( GeneType::Id, long, long );
};
