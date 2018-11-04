// gridField.h : 
//

#pragma once

#include <iostream>
#include "EvolutionTypes.h"
#include "gridPoint.h"
#include "interaction.h"
#include "individual.h"

class GridField
{
public:
    static void InitClass( )
	{
		m_iFertilizerYield = Config::GetConfigValue( Config::tId::fertilizerYield );
		m_iMaxFertilizer   = Config::GetConfigValue( Config::tId::maxFertilizer );
		m_sFoodReserve     = Config::GetConfigValueShort( Config::tId::reserveFood );
		m_sMaxFood         = Config::GetConfigValueShort( Config::tId::maxFood );
	}

	void InitGridFieldStructure( GridPoint const gp )
	{
		m_gp = gp;
	}

	void ResetGridField( short const sFood )
	{
		CutConnections( );
		m_sMutatRate  = 0;
		m_sFertility  = sFood;
		m_sFoodStock  = sFood;
		m_sFertilizer = 0;
		m_Individual.ResetIndividual( );
	}

	short const GetConsumption( short const sWant ) const 
	{
		short const sAvailable = m_sFoodStock - m_sFoodReserve;
		return ClipToMinMax( sAvailable, (short const) 0, sWant ); 
	}

    short          GetMutRate( )    const { return m_sMutatRate;  }
    short          GetFoodStock( )  const { return m_sFoodStock;  }
    short          GetFertility( )  const { return m_sFertility;  }
    short          GetFertilizer( ) const { return m_sFertilizer; }
    EVO_GENERATION GetGenBirth( )   const { return m_Individual.GetGenBirth( ); }
    tAction        GetLastAction( ) const { return m_Individual.GetLastAction( ); }
    IndId          GetId( )         const { return m_Individual.GetId( ); }
    tOrigin        GetOrigin( )     const { return m_Individual.GetOrigin( ); }
    short          GetEnergy( )     const { return m_Individual.GetEnergy( ); }
    bool           IsDead( )        const { return m_Individual.IsDead( ); }
    bool           IsAlive( )       const { return m_Individual.IsAlive( ); };
    bool           IsDefined( )     const { return m_Individual.IsDefined( ); };
    tStrategyId    GetStrategyId( ) const { return m_Individual.GetStrategyId( ); }
    MEM_INDEX      GetMemSize( )    const { return m_Individual.GetMemSize  ( ); }
    MEM_INDEX      GetMemUsed( )    const { return m_Individual.GetMemUsed( ); }
    Genome const & GetGenome( )     const { return m_Individual.GetGenome( ); }

    short  const   GetAllele( tGeneType const geneType ) const { return GetGenome( ).GetAllele( geneType ); }

    IndId GetMemEntry( MEM_INDEX const i ) const { return m_Individual.GetMemEntry( i ); }
    void  ResetIndividual( )                     { m_Individual.ResetIndividual( ); }
    void  SetEnergy( short const sInc )          { m_Individual.SetEnergy( sInc ); }
    void  DecEnergy( short const sDec )          { m_Individual.DecEnergy( sDec ); }
    void  IncEnergy( short const sInc )          { m_Individual.IncEnergy( sInc ); }
    void  SetLastAction( tAction const at )      { m_Individual.SetLastAction( at ); }

	void CreateIndividual( IndId const id, EVO_GENERATION const genBirth, tStrategyId const s )
	{
		m_Individual.Create( id, genBirth, s );
	}

	void Donate( GridField & gfDonator, short sDonation )
	{
		gfDonator.DecEnergy( sDonation );
		IncEnergy( sDonation );
	}

	void CloneIndividual( IndId const id, EVO_GENERATION const genBirth, Random & random, GridField & gfParent )
	{
		m_Individual.Clone( id, genBirth, m_sMutatRate, random, gfParent.m_Individual );
		long lDonationRate = static_cast<long>( gfParent.GetAllele( tGeneType::cloneDonation ) );
		long lParentEnergy = static_cast<long>( gfParent.GetEnergy( ) );
		long lDonation = ( lDonationRate * lParentEnergy ) / SHRT_MAX;
		Donate( gfParent, CastToShort( lDonation ) );
	}

	void BreedIndividual( IndId const id, EVO_GENERATION const genBirth, Random & random, GridField & gfParentA, GridField & gfParentB )
	{
		m_Individual.Breed( id, genBirth, m_sMutatRate, random, gfParentA.m_Individual, gfParentB.m_Individual );
		Donate( gfParentA, gfParentA.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
		Donate( gfParentB, gfParentB.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
	}

	void Fertilize( short const sInvest )
	{
		assert( sInvest > 0 );
		int const iYield    = (sInvest * m_iFertilizerYield ) / 100;
		int const iNewValue = min( m_sFertilizer + iYield, m_iMaxFertilizer ); 
		setFertilizer( CastToShort( iNewValue ) );
	}

	void PassOn2Child( IndId const id, EVO_GENERATION const genBirth, Random & random )
	{
		m_Individual.Clone( id, genBirth, m_sMutatRate, random, m_Individual );
	}

	void MoveIndividual( GridField & gfSrc )
	{
		m_Individual = gfSrc.m_Individual;
		gfSrc.m_Individual.ResetIndividual( );
	}

	void Apply2Fertilizer(short const s, ManipulatorFunc f) { setFertilizer( (f)( m_sFertilizer, s ) ); }
	void Apply2FoodStock (short const s, ManipulatorFunc f) { setFoodStock ( (f)( m_sFoodStock,  s ) ); }
	void Apply2Fertility (short const s, ManipulatorFunc f) { setFertility ( (f)( m_sFertility,  s ) ); }
	void Apply2MutRate   (short const s, ManipulatorFunc f) { setMutRate   ( (f)( m_sMutatRate,  s ) ); }

	void DecFoodStock( short const sDec )
	{ 
		ASSERT_SHORT_SUM( m_sFoodStock, - sDec );
		setFoodStock( m_sFoodStock - sDec ); 
	}

	void IncFoodStock( short const sInc )
	{ 
		ASSERT_SHORT_SUM( m_sFoodStock, sInc );
		setFoodStock( m_sFoodStock + sInc ); 
	}

    void ReduceFertilizer( ) { m_sFertilizer /= 2; }

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
		gfA.SetLastAction( tAction::interact );
	};

private:
    // data for management of neighborhood relation and list of living individuals

    GridPoint  m_gp;            //   4 byte     will stay unchanged after initialization
    GridPoint  m_gpSenior;      //   4 byte
    GridPoint  m_gpJunior;      //   4 byte

// configuraton data, changed only by user 

    short      m_sMutatRate;    //   2 byte
    short      m_sFertility;    //   2 byte     normal fertility of soil

// data changed by algorithm

    Individual m_Individual;    // 176 byte
    short      m_sFoodStock;    //   2 byte 
    short      m_sFertilizer;   //   2 byte
                    // sum         196 byte

// static members for caching frequently used configuration items

    static int   m_iFertilizerYield;
    static int   m_iMaxFertilizer;
    static short m_sFoodReserve;
    static short m_sMaxFood;

// private functions

	void setFertilizer( short const s ) { assert( s >= 0 ); m_sFertilizer = s; }
	void setFoodStock ( short const s ) { assert( s >= 0 ); m_sFoodStock  = s; }
	void setFertility ( short const s ) { assert( s >= 0 ); m_sFertility  = s; }
	void setMutRate   ( short const s ) { assert( s >= 0 ); m_sMutatRate  = min( s, (short)100 ); } // mutation rate is a percent value	
};

std::wostream & operator << ( std::wostream &, GridField const & );
