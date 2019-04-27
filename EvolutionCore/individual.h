// individual.h : 
//

#pragma once

#include "genome.h"
#include "strategy.h"
#include "StrategyData.h"
#include "plannedActivity.h"

//lint -sem(Individual::ResetIndividual,initializer)

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
    Action::Id      GetLastAction( )                       const { return m_action; }
    Genome const &  GetGenome    ( )                       const { return m_genome; }
	PlannedActivity GetPlan      ( )                       const { return m_plan; }
	MEM_INDEX       GetMemSize   ( )                       const { return m_stratData.GetMemSize( );  }
    MEM_INDEX       GetMemUsed   ( )                       const { return m_stratData.GetMemUsed( ); }
    short           GetAllele    ( GeneType::Id const gt ) const { return m_genome.GetAllele( gt ); }
    IND_ID          GetMemEntry  ( MEM_INDEX    const ui ) const { return m_stratData.GetMemEntry( ui ); }

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

	void SetPlan( PlannedActivity const & plan ) 
	{ 
		m_plan = plan; 
	}

	void SetEnergy( ENERGY_UNITS const energy )
	{
       //int sizIND_ID         = sizeof(IND_ID          );
       //int sizEVO_GENERATION = sizeof(EVO_GENERATION  );
       //int siztOrigin        = sizeof(tOrigin         );
       //int sizENERGY_UNITS   = sizeof(ENERGY_UNITS    );
       //int sizStrategyData   = sizeof(StrategyData    );
       //int sizGenome         = sizeof(Genome          );
       //int sizActionId       = sizeof(Action::Id      );
       //int sizIndividual     = sizeof(Individual      );
	   m_enStock = ( energy > m_enCapacity ) ? m_enCapacity : energy;
	}

	void IncEnergy( ENERGY_UNITS const sInc )
	{
		SetEnergy( ENERGY_UNITS( AssertShortSum( m_enStock.GetValue(), sInc.GetValue() ) ) );
	}

private:
    StrategyData     m_stratData;   // 40 bytes
	Strategy const * m_pStrategy;   //  8 bytes 
	PlannedActivity  m_plan;        // 18 byte   
	IND_ID           m_id;          //  4 bytes
    EVO_GENERATION   m_genBirth;    //  4 bytes
    tOrigin          m_origin;      //  2 bytes
    ENERGY_UNITS     m_enCapacity;  //  2 bytes
	Action::Id       m_action;      //  2 bytes
    ENERGY_UNITS     m_enStock;     //  2 bytes
    Genome           m_genome;      // 30 bytes
//	short            padding;       //  2 bytes 
                             // sum:  114 bytes

	static const std::unordered_map< Strategy::Id, Strategy * const > m_apStrat; 

    static ENERGY_UNITS m_stdEnergyCapacity;
    static ENERGY_UNITS m_initialEnergy;
};
        