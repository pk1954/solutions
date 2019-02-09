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
    void    InitGenome( );
    tAction GetOption( bool const, bool const, int const, EVO_GENERATION const, Random & ) const;
    void    Mutate( short const, Random & );
    void    Recombine( Genome const &, Genome const &, Random & );

    static void InitClass( );

    // for statistics

	short GetDistr( tAction const action ) const
	{ 
		return m_aGeneActions.at( static_cast<int>( action ) ).m_gene.GetAllele();
	}

    short GetAllele( tGeneType const geneType ) const 
	{ 
		return m_aGeneGeneral[ static_cast<int>( geneType ) ].m_gene.GetAllele(); 
	};

	static bool IsEnabled( tAction const action ) { return enabled( action ); };

private:
 
    struct generalGene
    {
        tGeneType m_type;
        Gene      m_gene;
    };

    struct actionGene
    {
        tAction m_action;
        Gene    m_gene;
    };

    std::array< generalGene, NR_GENES        > m_aGeneGeneral;  // 4 * 9 = 36   general genes 
    std::array< actionGene,  NR_ACTION_GENES > m_aGeneActions;  // 4 * 8 = 32   genes for choosing next action
                                                           // sum     68
    
    void setGeneralGene( tGeneType const, int const );
    void setActionGene ( tAction   const, int const );

	// static members and functions

	static unsigned int const MAX_LIFE_SPAN = 200;

	static std::array< bool,         NR_ACTIONS        > m_abActionEnabled;
	static std::array< unsigned int, MAX_LIFE_SPAN + 1 > m_mortalityTable;

	static bool & enabled( tAction const action ) { return m_abActionEnabled[ static_cast<unsigned short>( action ) ]; 	}

    static Genome m_genomeTemplate;

    static std::array< GeneTypeLimits, NR_GENES        > m_aLimitsGeneral;
    static std::array< GeneTypeLimits, NR_ACTION_GENES > m_aLimitsActions;

    static void setGeneralLimits( tGeneType, long, long );
};
