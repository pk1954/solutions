// genome.h : 
//

#pragma once

#include <array>
#include "gene.h"
#include "config.h"
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

	short GetDistr( GeneType::Id const actionGene ) const
	{ 
		return m_aGene.at( static_cast<int>( actionGene ) ).m_gene.GetAllele();
	}

    short GetAllele( GeneType::Id const geneType ) const 
	{ 
		return m_aGene.at( static_cast<int>( geneType ) ).m_gene.GetAllele(); 
	};

    short GetAllele( Action::Id const action ) const 
	{ 
		return GetAllele( GetRelatedGeneType( action ) ); 
	};

	static bool IsEnabled( Action::Id const action ) 
	{ 
		return enabled( action ); 
    };

private:
 
    struct GeneStruct { GeneType::Id m_type; Gene m_gene; } ;

    std::array< GeneStruct, GeneType ::COUNT > m_aGene;  
                                                                   
    void setGene( GeneType::Id const, short const );

	// static members and functions

	static unsigned int const MAX_LIFE_SPAN = 200;
	static std::array< unsigned int, MAX_LIFE_SPAN + 1 > m_mortalityTable;

	static std::array< bool, Action::COUNT > m_abActionEnabled;
	static bool & enabled( Action::Id const action ) 
	{ 
		return m_abActionEnabled.at( static_cast<unsigned short>( action ) ); 	
	}

    static Genome        m_genomeTemplate;
	static ActionOptions m_options;

    static std::array< GeneTypeLimits, GeneType::COUNT > m_aLimits;

    static void setLimits( GeneType::Id, long, long );
};
