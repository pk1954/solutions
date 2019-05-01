// individual.h : 
//

#pragma once

#include <array>
#include "genome.h"
#include "strategy.h"
#include "StrategyData.h"

class Random;

class Individual
{
public:
    
    static void RefreshCache( );

    Individual( );

    void ResetIndividual( );
    
    ENERGY_UNITS    GetEnergy    ( )                       const { return m_enStock; };
    EVO_GENERATION  GetGenBirth  ( )                       const { return m_genBirth; };
    bool            IsDead       ( )                       const { return m_enStock <= 0_ENERGY_UNITS; };
    bool            IsAlive      ( )                       const { return m_enStock >  0_ENERGY_UNITS; };
    bool            IsDefined    ( )                       const { return m_id.IsNotNull(); };
    IND_ID          GetId        ( )                       const { return m_id; };
    tOrigin         GetOrigin    ( )                       const { return m_origin; }
    Genome const &  GetGenome    ( )                       const { return m_genome; }
	Strategy::Id    GetStrategyId( )                       const { return m_strategyId; }
	MEM_INDEX       GetMemSize   ( )                       const { return m_stratData.GetMemSize( );  }
    MEM_INDEX       GetMemUsed   ( )                       const { return m_stratData.GetMemUsed( ); }
    short           GetAllele    ( GeneType::Id const gt ) const { return m_genome.GetAllele( gt ); }
    IND_ID          GetMemEntry  ( MEM_INDEX    const ui ) const { return m_stratData.GetMemEntry( ui ); }

    void Create( IND_ID const, EVO_GENERATION const, Strategy::Id const );
    void Clone ( IND_ID const, EVO_GENERATION const, PERCENT const, Random &, Individual const & );
    void Breed ( IND_ID const, EVO_GENERATION const, PERCENT const, Random &, Individual const &, Individual const & );

	void Remember( IND_ID const & partnerId, bool const bPartnerReaction ) 
	{ 
		m_apStrat.at( static_cast<int>(m_strategyId) )->Remember( m_stratData, partnerId, bPartnerReaction );
	};

	bool InteractWith( IND_ID const & partnerId ) 
	{ 
		return m_apStrat.at( static_cast<int>(m_strategyId) )->InteractWith( m_stratData, partnerId );
	};

	void SetEnergy( ENERGY_UNITS const energy )
	{
       //int sizIND_ID         = sizeof(IND_ID          );
       //int sizEVO_GENERATION = sizeof(EVO_GENERATION  );
       //int siztOrigin        = sizeof(tOrigin         );
       //int sizENERGY_UNITS   = sizeof(ENERGY_UNITS    );
       //int sizStrategyData   = sizeof(StrategyData    );
       //int sizGenome         = sizeof(Genome          );
       //int sizActionId       = sizeof(Action::Id      );
       int sizIndividual     = sizeof(Individual      );
	   m_enStock = ( energy > m_enCapacity ) ? m_enCapacity : energy;
	}

	void IncEnergy( ENERGY_UNITS const sInc )
	{
		SetEnergy( ENERGY_UNITS( AssertShortSum( m_enStock.GetValue(), sInc.GetValue() ) ) );
	}

	Action::Id GetOption
	(
		bool           const bHasFreeSpace,
		bool           const bHasNeighbor,
		ENERGY_UNITS   const energy,
		EVO_GENERATION const age,
		Random             & random
	) const
	{
		return m_genome.GetOption( bHasFreeSpace, bHasNeighbor, energy, age, random );
	}

private:
    StrategyData   m_stratData;   // 40 bytes
	IND_ID         m_id;          //  4 bytes
	EVO_GENERATION m_genBirth;    //  4 bytes
    Genome         m_genome;      // 30 bytes
	Strategy::Id   m_strategyId;  //  2 bytes 
	tOrigin        m_origin;      //  2 bytes
    ENERGY_UNITS   m_enCapacity;  //  2 bytes
    ENERGY_UNITS   m_enStock;     //  2 bytes
                            // sum:  88 bytes

	static const std::array< Strategy * const, Strategy::COUNT > m_apStrat; 

    static ENERGY_UNITS m_stdEnergyCapacity;
    static ENERGY_UNITS m_initialEnergy;
};
        