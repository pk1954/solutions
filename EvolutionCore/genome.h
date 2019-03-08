// genome.h : 
//

#pragma once

#include <array>
#include "gene.h"
#include "config.h"
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

	short GetDistr( ActionGeneType::Id const actionGene ) const
	{ 
		return m_aGeneActions.at( static_cast<int>( actionGene ) ).m_gene.GetAllele();
	}

    short GetAllele( GeneralGeneType::Id const geneType ) const 
	{ 
		return m_aGeneGeneral.at( static_cast<int>( geneType ) ).m_gene.GetAllele(); 
	};

	static bool IsEnabled( Action::Id const action ) 
	{ 
		return enabled( action ); 
};

private:
 
    struct generalGene
    {
        GeneralGeneType::Id m_type;
        Gene                m_gene;
    };

    struct actionGene
    {
        ActionGeneType::Id m_type;
        Gene               m_gene;
    };
                                          
    std::array< actionGene,  ActionGeneType ::COUNT > m_aGeneActions;  // 4 * 6 = 24   genes for choosing next action
    std::array< generalGene, GeneralGeneType::COUNT > m_aGeneGeneral;  // 4 * 9 = 36   general genes 
                                                                       // sum     66
    void setGeneralGene( GeneralGeneType::Id const, short const );
    void setActionGene ( ActionGeneType::Id  const, short const );

	// static members and functions

	static unsigned int const MAX_LIFE_SPAN = 200;
	static std::array< unsigned int, MAX_LIFE_SPAN + 1 > m_mortalityTable;

	static std::array< bool, Action::COUNT > m_abActionEnabled;
	static bool & enabled( Action::Id const action ) 
	{ 
		return m_abActionEnabled.at( static_cast<unsigned short>( action ) ); 	
	}

    static Genome m_genomeTemplate;

    static std::array< GeneTypeLimits, GeneralGeneType::COUNT > m_aLimitsGeneral;
    static std::array< GeneTypeLimits, ActionGeneType ::COUNT > m_aLimitsActions;

    static void setGeneralLimits( GeneralGeneType::Id, long, long );
};
