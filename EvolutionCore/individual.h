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
    
    ENERGY_UNITS   GetEnergy    ( )                       const { return m_enStock; };
    EVO_GENERATION GetGenBirth  ( )                       const { return m_genBirth; };
    bool           IsDead       ( )                       const { return m_enStock <= 0_ENERGY_UNITS; };
    bool           IsAlive      ( )                       const { return m_enStock >  0_ENERGY_UNITS; };
    bool           IsDefined    ( )                       const { return m_id.IsNotNull(); };
    IND_ID         GetId        ( )                       const { return m_id; };
    tOrigin        GetOrigin    ( )                       const { return m_origin; }
    Action::Id     GetLastAction( )                       const { return m_action; }
    Genome const & GetGenome    ( )                       const { return m_genome; }
    MEM_INDEX      GetMemSize   ( )                       const { return m_stratData.GetMemSize( );  }
    MEM_INDEX      GetMemUsed   ( )                       const { return m_stratData.GetMemUsed( ); }
    short          GetAllele    ( GeneType::Id const gt ) const { return m_genome.GetAllele( gt ); }
    IND_ID         GetMemEntry  ( MEM_INDEX    const ui ) const { return m_stratData.GetMemEntry( ui ); }

	Strategy::Id GetStrategyId( ) const { return m_pStrategy->GetStrategyId(); }

    void Create( IND_ID const, EVO_GENERATION const, Strategy::Id const );
    void Clone ( IND_ID const, EVO_GENERATION const, PERCENT const, Random &, Individual const & );
    void Breed ( IND_ID const, EVO_GENERATION const, PERCENT const, Random &, Individual const &, Individual const & );

	void Remember( IND_ID const & partnerId, bool const bPartnerReaction ) 
	{ 
		m_pStrategy->Remember( m_stratData, partnerId, bPartnerReaction );
	};

	bool InteractWith( IND_ID const & partnerId ) 
	{ 
		return m_pStrategy->InteractWith( m_stratData, partnerId );
	};

	void SetLastAction( Action::Id const action ) 
	{ 
		m_action = action; 
	}

	void SetEnergy( ENERGY_UNITS const energy )
	{
	   m_enStock = ( energy > m_enCapacity ) ? m_enCapacity : energy;
	}

	void IncEnergy( ENERGY_UNITS const sInc )
	{
		SetEnergy( ENERGY_UNITS( AssertShortSum( m_enStock.GetValue(), sInc.GetValue() ) ) );
	}

private:
    IND_ID           m_id;          //  4 bytes
    EVO_GENERATION   m_genBirth;    //  4 bytes
    tOrigin          m_origin;      //  2 bytes
    ENERGY_UNITS     m_enCapacity;  //  2 bytes
    StrategyData     m_stratData;   // 84 bytes
    Genome           m_genome;      // 68 bytes
	Strategy const * m_pStrategy;   //  8 bytes 
	Action::Id       m_action;      //  2 bytes
    ENERGY_UNITS     m_enStock;     //  2 bytes
                             // sum:  176 bytes

	static const std::unordered_map< Strategy::Id, Strategy * const > m_apStrat; 

    static ENERGY_UNITS m_stdEnergyCapacity;
    static ENERGY_UNITS m_initialEnergy;
};
        