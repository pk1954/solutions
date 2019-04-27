// gridField.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridPoint.h"
#include "plannedActivity.h"
#include "interaction.h"
#include "individual.h"

class GridField
{
public:
	static void RefreshCash( )
	{
		m_lFertilizerYield = Config::GetConfigValue( Config::tId::fertilizerYield );
		m_enMaxFertilizer  = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::maxFertilizer ));
		m_enFoodReserve    = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::reserveFood ));
		m_enMaxFood        = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::maxFood ));
	}

	GridField::GridField() :
		m_gp      ( GridPoint::NULL_VAL() ),
		m_gpSenior( GridPoint::NULL_VAL() ),
		m_gpJunior( GridPoint::NULL_VAL() ),   
		m_mutRate(),   
		m_enFertility(), 
		m_Individual(), 
		m_enFoodStock(), 
		m_enFertilizer() 
	{}

	void InitGridFieldStructure( GridPoint const gp )
	{
		m_gp = gp;
	}

	void ResetGridField( ENERGY_UNITS const enFood )
	{
		CutConnections( );
		m_mutRate      = 30_PERCENT;
		m_enFertility  = enFood;
		m_enFoodStock  = enFood;
		m_enFertilizer = 0_ENERGY_UNITS;
		m_Individual.ResetIndividual( );
	}

	ENERGY_UNITS const GetConsumption( ENERGY_UNITS const sWant ) const 
	{
		ENERGY_UNITS const available = m_enFoodStock - m_enFoodReserve;
		return ENERGY_UNITS( ClipToMinMax( available, 0_ENERGY_UNITS, sWant ) ); 
	}

	PERCENT           GetMutRate( )      const { return m_mutRate;  }
	ENERGY_UNITS      GetFoodStock( )    const { return m_enFoodStock;  }
    ENERGY_UNITS      GetFertility( )    const { return m_enFertility;  }
    ENERGY_UNITS      GetFertilizer( )   const { return m_enFertilizer; }
    EVO_GENERATION    GetGenBirth( )     const { return m_Individual.GetGenBirth( ); }
    Action::Id        GetLastAction( )   const { return m_Individual.GetLastAction( ); }
    IND_ID            GetId( )           const { return m_Individual.GetId( ); }
    tOrigin           GetOrigin( )       const { return m_Individual.GetOrigin( ); }
    ENERGY_UNITS      GetEnergy( )       const { return m_Individual.GetEnergy( ); }
    bool              IsDead( )          const { return m_Individual.IsDead( ); }
    bool              IsAlive( )         const { return m_Individual.IsAlive( ); };
    bool              IsDefined( )       const { return m_Individual.IsDefined( ); };
    MEM_INDEX         GetMemSize( )      const { return m_Individual.GetMemSize( ); }
    MEM_INDEX         GetMemUsed( )      const { return m_Individual.GetMemUsed( ); }
    Genome const &    GetGenome( )       const { return m_Individual.GetGenome( ); }
    Strategy::Id      GetStrategyId( )   const { return m_Individual.GetStrategyId( ); }
	PlannedActivity   GetPlan( )         const { return m_Individual.GetPlan( );  }

	short  GetAllele( GeneType::Id const geneType ) const { return GetGenome( ).GetAllele( geneType ); }

    IND_ID GetMemEntry( MEM_INDEX const i ) const { return m_Individual.GetMemEntry( i ); }

    void   ResetIndividual( )                     { m_Individual.ResetIndividual( ); }
    void   SetEnergy( ENERGY_UNITS const sInc )   { m_Individual.SetEnergy( sInc ); }
    void   DecEnergy( ENERGY_UNITS const sDec )   { m_Individual.IncEnergy( - sDec ); }
    void   IncEnergy( ENERGY_UNITS const sInc )   { m_Individual.IncEnergy( sInc ); }
    void   SetLastAction( Action::Id const at )   { m_Individual.SetLastAction( at ); }

	void CreateIndividual( IND_ID const id, EVO_GENERATION const genBirth, Strategy::Id const s )
	{
		m_Individual.Create( id, genBirth, s );
	}

	void Donate( GridField & gfDonator, ENERGY_UNITS sDonation )
	{
		gfDonator.DecEnergy( sDonation );
		IncEnergy( sDonation );
	}

	void CloneIndividual( IND_ID const id, EVO_GENERATION const genBirth, Random & random, GridField & gfParent )
	{
		m_Individual.Clone( id, genBirth, m_mutRate, random, gfParent.m_Individual );
		long lDonationRate = static_cast<long>( gfParent.GetAllele( GeneType::Id::cloneDonation ) );
		long lParentEnergy = static_cast<long>( gfParent.GetEnergy( ).GetValue() );
		long lDonation = ( lDonationRate * lParentEnergy ) / SHRT_MAX;
		Donate( gfParent, ENERGY_UNITS(CastToShort( lDonation )) );
	}

	void BreedIndividual( IND_ID const id, EVO_GENERATION const genBirth, Random & random, GridField & gfParentA, GridField & gfParentB )
	{
		m_Individual.Breed( id, genBirth, m_mutRate, random, gfParentA.m_Individual, gfParentB.m_Individual );
		Donate( gfParentA, gfParentA.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
		Donate( gfParentB, gfParentB.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
	}

	void Fertilize( ENERGY_UNITS const enInvest )
	{
		assert( enInvest > 0_ENERGY_UNITS );
		ENERGY_UNITS const yield    = (enInvest * m_lFertilizerYield ) / 100;
		ENERGY_UNITS const newValue = std::min( m_enFertilizer + yield, m_enMaxFertilizer ); 
		setFertilizer( newValue );
	}

	void PassOn2Child( IND_ID const id, EVO_GENERATION const genBirth, Random & random )
	{
		m_Individual.Clone( id, genBirth, m_mutRate, random, m_Individual );
	}

	void MoveIndividual( GridField & gfSrc )
	{
		m_Individual = gfSrc.m_Individual;
		gfSrc.m_Individual.ResetIndividual( );
	}

	void Apply2MutRate   (PERCENT      const s, ManipulatorFunc f) { setMutRate   ( PERCENT     ((f)( m_mutRate.GetValue(),      s.GetValue() ) ) ); }
	void Apply2Fertilizer(ENERGY_UNITS const s, ManipulatorFunc f) { setFertilizer( ENERGY_UNITS((f)( m_enFertilizer.GetValue(), s.GetValue() ) ) ); }
	void Apply2FoodStock (ENERGY_UNITS const s, ManipulatorFunc f) { setFoodStock ( ENERGY_UNITS((f)( m_enFoodStock.GetValue(),  s.GetValue() ) ) ); }
	void Apply2Fertility (ENERGY_UNITS const s, ManipulatorFunc f) { setFertility ( ENERGY_UNITS((f)( m_enFertility.GetValue(),  s.GetValue() ) ) ); }

	void IncFoodStock( ENERGY_UNITS const sInc )
	{ 
		setFoodStock( ENERGY_UNITS( AssertShortSum( m_enFoodStock.GetValue(), sInc.GetValue() ) ) ); 
	}

    void ReduceFertilizer( ) { m_enFertilizer /= 2; }

    GridPoint const GetGridPoint( ) const { return m_gp; }
    GridPoint const GetSeniorGp ( ) const { return m_gpSenior; }
    GridPoint const GetJuniorGp ( ) const { return m_gpJunior; }

    bool IsOldest  ( ) const { return m_gpSenior.IsNull( ); }
    bool IsYoungest( ) const { return m_gpJunior.IsNull( ); }

    void SetSeniorGp( GridPoint const gp ) { m_gpSenior = gp; }
    void SetJuniorGp( GridPoint const gp ) { m_gpJunior = gp; }

	void CutConnections( )
	{
		m_gpJunior.Set2Null( );
		m_gpSenior.Set2Null( );
	}

	static void Interact( GridField & gfA, GridField & gfB )
	{
		INTERACTION::Interact( gfA.m_Individual, gfB.m_Individual );
		gfA.SetLastAction( Action::Id::interact );
	};

	void SetPlan( PlannedActivity const & plan ) 
	{ 
		m_Individual.SetPlan( plan ); 
	};

private:
    // data for management of neighborhood relation and list of living individuals

    Individual      m_Individual;   // 114 byte    changed by algorithm

	GridPoint       m_gp;           //   4 byte     will stay unchanged after initialization
    GridPoint       m_gpSenior;     //   4 byte     will stay unchanged after initialization
    GridPoint       m_gpJunior;     //   4 byte     will stay unchanged after initialization
				    
    ENERGY_UNITS    m_enFoodStock;  //   2 byte    changed by algorithm
    ENERGY_UNITS    m_enFertilizer; //   2 byte    changed by algorithm

// configuraton data, changed only by user 

    PERCENT         m_mutRate;      //   2 byte   
    ENERGY_UNITS    m_enFertility;  //   2 byte     normal fertility of soil
                         // sum        134 byte

// static members for caching frequently used configuration items

    static long         m_lFertilizerYield;
    static ENERGY_UNITS m_enMaxFertilizer;
    static ENERGY_UNITS m_enFoodReserve;
    static ENERGY_UNITS m_enMaxFood;

// private functions

	void setFertilizer( ENERGY_UNITS const s ) { assert( s >= 0_ENERGY_UNITS ); m_enFertilizer = s; }
	void setFoodStock ( ENERGY_UNITS const s ) { assert( s >= 0_ENERGY_UNITS ); m_enFoodStock  = s; }
	void setFertility ( ENERGY_UNITS const s ) { assert( s >= 0_ENERGY_UNITS ); m_enFertility  = s; }
	void setMutRate   ( PERCENT      const s ) { assert( s >= 0_PERCENT       ); m_mutRate = PERCENT{ std::min( s.GetValue(), (short)100 ) }; }
};

std::wostream & operator << ( std::wostream &, GridField const & );
