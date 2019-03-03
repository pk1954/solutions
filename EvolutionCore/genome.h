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
    Action::Id GetOption( bool const, bool const, int const, EVO_GENERATION const, Random & ) const;
    void       Mutate( short const, Random & );
    void       Recombine( Genome const &, Genome const &, Random & );

    static void InitClass( );

    // for statistics

	short GetDistr( Action::Id const action ) const
	{ 
		return m_aGeneActions.at( static_cast<int>( action ) ).m_gene.GetAllele();
	}

    short GetAllele( tGeneType const geneType ) const 
	{ 
		return m_aGeneGeneral[ static_cast<int>( geneType ) ].m_gene.GetAllele(); 
	};

	static bool IsEnabled( Action::Id const action ) { return enabled( action ); };

private:
 
    struct generalGene
    {
        tGeneType m_type;
        Gene      m_gene;
    };

    struct actionGene
    {
        Action::Id m_action;
        Gene       m_gene;
    };

    std::array< generalGene, NR_GENES                > m_aGeneGeneral;  // 4 * 9 = 36   general genes 
    std::array< actionGene,  Action::NR_ACTION_GENES > m_aGeneActions;  // 4 * 8 = 32   genes for choosing next action
                                                                // sum     68
    
    void setGeneralGene( tGeneType  const, int const );
    void setActionGene ( Action::Id const, int const );

	// static members and functions

	static unsigned int const MAX_LIFE_SPAN = 200;

	static std::array< bool,         Action::NR_ACTIONS > m_abActionEnabled;
	static std::array< unsigned int, MAX_LIFE_SPAN + 1 > m_mortalityTable;

	static bool & enabled( Action::Id const action ) { return m_abActionEnabled[ static_cast<unsigned short>( action ) ]; 	}

    static Genome m_genomeTemplate;

    static std::array< GeneTypeLimits, NR_GENES                > m_aLimitsGeneral;
    static std::array< GeneTypeLimits, Action::NR_ACTION_GENES > m_aLimitsActions;

    static void setGeneralLimits( tGeneType, long, long );
};
