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
    static void InitClass( );

    void InitGridFieldStructure( GridPoint const & );
    void ResetGridField( short const );

    void        Fertilize     ( short const );
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

    long GetMemEntry( MEM_INDEX const i ) const { return m_Individual.GetMemEntry( i ).GetLong( ); }
    void ResetIndividual( )                     { m_Individual.ResetIndividual( ); }
    void SetEnergy( short const sInc )          { m_Individual.SetEnergy( sInc ); }
    void DecEnergy( short const sDec )          { m_Individual.DecEnergy( sDec ); }
    void IncEnergy( short const sInc )          { m_Individual.IncEnergy( sInc ); }
    void SetLastAction( tAction const at )      { m_Individual.SetLastAction( at ); }

    void Donate( GridField &, short  );
    void CreateIndividual( IndId const, EVO_GENERATION const, tStrategyId const );
    void CloneIndividual ( IndId const, EVO_GENERATION const, Random &, GridField & );
    void BreedIndividual ( IndId const, EVO_GENERATION const, Random &, GridField &, GridField & );
    void MoveIndividual  ( GridField & );

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

    GridPoint const & GetGridPoint( ) const { return m_gp; }
    GridPoint const & GetSeniorGp ( ) const { return m_gpSenior; }
    GridPoint const & GetJuniorGp ( ) const { return m_gpJunior; }

    bool IsOldest  ( ) const { return m_gpSenior.IsNull( ); }
    bool IsYoungest( ) const { return m_gpJunior.IsNull( ); }

    void SetSeniorGp( GridPoint const & gp ) { m_gpSenior = gp; }
    void SetJuniorGp( GridPoint const & gp ) { m_gpJunior = gp; }

    void CutConnections( );

    static void Interact( GridField &, GridField & );

private:
    // data for management of neighborhood relation and list of living individuals

    GridPoint  m_gp;            //   4 byte     will stay unchanged after initialization
    GridPoint  m_gpSenior;      //   4 byte
    GridPoint  m_gpJunior;      //   4 byte

// configuraton data, changed only by user 

    short      m_sMutatRate;    //   2 byte
    short      m_sFertility;    //   2 byte     normal fertility of soil

// data changed by algorithm

    Individual m_Individual;    // 162 byte
    short      m_sFoodStock;    //   2 byte 
    short      m_sFertilizer;   //   2 byte
                    // sum         186 byte

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

class Decision_Functor
{
public:
    virtual ~Decision_Functor() {};
    virtual bool operator() ( GridField const & ) const = 0; 
};

class IsDead_Functor : public Decision_Functor
{
public:
    virtual bool operator() ( GridField const & gf ) const { return gf.IsDead( ); }; 
};

class IsAlive_Functor : public Decision_Functor
{
public:
    virtual bool operator() ( GridField const & gf ) const { return gf.IsAlive( ); }; 
};

class IsCompatible_Functor : public Decision_Functor
{
public:
    explicit IsCompatible_Functor( tStrategyId const strat ) : m_strat( strat ) { };
    virtual ~IsCompatible_Functor() {};
    virtual bool operator() ( GridField const & gf ) const 
    { 
        return gf.IsAlive() && ( gf.GetStrategyId() == m_strat ); 
    }; 
private:
    tStrategyId m_strat;
};
