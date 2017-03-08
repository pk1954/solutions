// individual.h : 
//

#pragma once

#include "random.h"
#include "strategy.h"
#include "genome.h"
#include "EvolutionTypes.h"

//lint -sem(Individual::ResetIndividual,initializer)

class Individual
{
public:
    
    static void InitClass( );

    Individual( );

    void ResetIndividual( );
    
    short          GetEnergy  ( ) const { return m_sEnergy; };
    EVO_GENERATION GetGenBirth( ) const { return m_genBirth; };
    bool           IsDead     ( ) const { return m_sEnergy <= 0; };
    bool           IsAlive    ( ) const { return m_sEnergy >  0; };
    IndId          GetId      ( ) const { return m_id; };
    tOrigin        GetOrigin  ( ) const { return m_origin; }

    Genome const & GetGenome  ( )                         const { return m_genome; }
    tStrategyId    GetStrategyId( )                       const { return m_strategyId; }
    MEM_INDEX      GetMemSize  ( )                        const { return m_strat.GetMemSize( );  }
    MEM_INDEX      GetMemUsed( )                          const { return m_strat.GetMemUsed( ); }
    short          GetAllele( tGeneType const geneType )  const { return m_genome.GetAllele( geneType ); }
    IndId          GetMemEntry( MEM_INDEX const ui )      const { return m_strat.GetMemEntry( ui ); }
    tAction        GetLastAction( )                       const { return m_at; }

    bool InteractWith( IndId const & );
    void Remember    ( IndId const &, bool const );
    
    void Create( IndId const, EVO_GENERATION const, tStrategyId const );
    void Clone ( IndId const, EVO_GENERATION const, short const, Random &, Individual const & );
    void Breed ( IndId const, EVO_GENERATION const, short const, Random &, Individual const &, Individual const & );

    void IncEnergy( short const );
    void DecEnergy( short const );
    void SetEnergy( short const );

    void SetLastAction( tAction const at ) { m_at = at; }

private:
    IndId          m_id;          //  4 bytes
    EVO_GENERATION m_genBirth;    //  4 bytes
    tOrigin        m_origin;      //  2 bytes
    short          m_sCapacity;   //  2 bytes
    StrategyData   m_strat;       // 84 bytes
    Genome         m_genome;      // 30 bytes
    tStrategyId    m_strategyId;  //  2 bytes
    tAction        m_at;          //  2 bytes
    short          m_sEnergy;     //  2 bytes
                           // sum:  132 bytes

    static short m_sStdEnergyCapacity;
    static short m_sInitialEnergy;
};
        