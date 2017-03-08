// genome.h : 
//

#pragma once

#include <array>
#include "gene.h"
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

private:
 
    void setGeneralGene( tGeneType const, int const );
    void setActionGene ( tAction   const, int const );

    struct actionGene
    {
        tAction m_action;
        Gene    m_gene;
    };

    struct generalGene
    {
        tGeneType m_type;
        Gene      m_gene;
    };

    array< generalGene, NR_GENES   > m_aGeneGeneral;  // 4 * 9 = 36   general genes 
    array< actionGene,  NR_ACTIONS > m_aGeneActions;  // 4 * 6 = 24   genes for choosing next action
                                                      // sum     60
    
    static bool m_bMoveEnabled;
    static bool m_bFertilizeEnabled;
    static bool m_bCloneEnabled;
    static bool m_bMarryEnabled;
    static bool m_bInteractEnabled;
    static bool m_bEatEnabled;

    static Genome m_genomeTemplate;

    static array< GeneTypeLimits, NR_GENES   > m_aLimitsGeneral;
    static array< GeneTypeLimits, NR_ACTIONS > m_aLimitsActions;

    static void setGeneralLimits( tGeneType, long, long );
};
