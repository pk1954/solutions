// genome.h : 
//

#pragma once

#include <array>
#include "gene.h"
#include "config.h"
#include "EvolutionTypes.h"

using namespace std;

class Random;

//lint -sem(Genome::init,initializer)

class Genome
{
public:
    
    Genome( );
    void    InitGenome( );
    tAction GetOption( bool const, bool const, int const, Random & ) const;
    void    Mutate( short const, Random & );
    void    Recombine( Genome const &, Genome const &, Random & );

    static void InitClass( );

    // for statistics
    short GetDistr ( tAction const   ) const;
    short GetAllele( tGeneType const geneType ) const { return m_aGeneGeneral[ static_cast<int>( geneType ) ].m_gene.GetAllele(); };

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

    array< generalGene, NR_GENES   > m_aGeneGeneral;  // 4 * 9 = 36   general genes 
    array< actionGene,  NR_ACTIONS > m_aGeneActions;  // 4 * 6 = 24   genes for choosing next action
                                                      // sum     60
    
    void setGeneralGene( tGeneType const, int const );
    void setActionGene ( tAction   const, int const );

	// static members and functions

	static array< bool, NR_ACTIONS > m_abActionEnabled;

	static bool & enabled( tAction const action ) { return m_abActionEnabled[ static_cast<unsigned short>( action ) ]; 	}

    static Genome m_genomeTemplate;

    static array< GeneTypeLimits, NR_GENES   > m_aLimitsGeneral;
    static array< GeneTypeLimits, NR_ACTIONS > m_aLimitsActions;

    static void setGeneralLimits( tGeneType, long, long );
};
