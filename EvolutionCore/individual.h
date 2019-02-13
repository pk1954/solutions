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
    
    short          GetEnergy    ( )                    const { return m_sEnergy; };
    EVO_GENERATION GetGenBirth  ( )                    const { return m_genBirth; };
    bool           IsDead       ( )                    const { return m_sEnergy <= 0; };
    bool           IsAlive      ( )                    const { return m_sEnergy >  0; };
    bool           IsDefined    ( )                    const { return m_id.IsNotNull(); };
    IND_ID         GetId        ( )                    const { return m_id; };
    tOrigin        GetOrigin    ( )                    const { return m_origin; }
    tAction        GetLastAction( )                    const { return m_at; }
    Genome const & GetGenome    ( )                    const { return m_genome; }
    tStrategyId    GetStrategyId( )                    const { return m_pStrategy->GetStrategyId(); }
    MEM_INDEX      GetMemSize   ( )                    const { return m_strat.GetMemSize( );  }
    MEM_INDEX      GetMemUsed   ( )                    const { return m_strat.GetMemUsed( ); }
    short          GetAllele    ( tGeneType const gt ) const { return m_genome.GetAllele( gt ); }
    IND_ID         GetMemEntry  ( MEM_INDEX const ui ) const { return m_strat.GetMemEntry( ui ); }
	
    void Create( IND_ID const, EVO_GENERATION const, tStrategyId const );
    void Clone ( IND_ID const, EVO_GENERATION const, short const, Random &, Individual const & );
    void Breed ( IND_ID const, EVO_GENERATION const, short const, Random &, Individual const &, Individual const & );

	void Remember( IND_ID const & partnerId, bool const bPartnerReaction ) 
	{ 
		m_pStrategy->Remember( m_strat, partnerId, bPartnerReaction );
	};

	bool InteractWith( IND_ID const & partnerId ) 
	{ 
		return m_pStrategy->InteractWith( m_strat, partnerId );
	};

	void SetLastAction( tAction const at ) 
	{ 
		m_at = at; 
	}

	void SetEnergy( short const energy )
	{
	   m_sEnergy = ( energy > m_sCapacity ) ? m_sCapacity : energy;
	}

	void IncEnergy( short const sInc )
	{
		SetEnergy( AssertShortSum( m_sEnergy, sInc ) );
	}

private:
    IND_ID           m_id;          //  4 bytes
    EVO_GENERATION   m_genBirth;    //  4 bytes
    tOrigin          m_origin;      //  2 bytes
    short            m_sCapacity;   //  2 bytes
    StrategyData     m_strat;       // 84 bytes
    Genome           m_genome;      // 68 bytes
	Strategy const * m_pStrategy;   //  8 bytes 
	tAction          m_at;          //  2 bytes
    short            m_sEnergy;     //  2 bytes
                             // sum:  176 bytes

	static const std::unordered_map< tStrategyId, Strategy * const > m_apStrat; 

    static short m_sStdEnergyCapacity;
    static short m_sInitialEnergy;
};
        