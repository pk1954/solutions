// grid_model.h : 
//

#pragma once

#include <array> 
#include <vector> 
#include <algorithm>
#include "random.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gpList.h"
#include "gridField.h"
#include "gridPOI.h"
#include "GridDimensions.h"
#include "EvolutionTypes.h"
#include "plannedActivity.h"

using std::array;
using std::vector;

class GridCircle;
class Manipulator;
class EventInterface;
class ObserverInterface;

using GROWTH_RATE = NamedType< int, struct GROWTH_RATE_Parameter >;

class Grid
{
public:

	static void InitClass( ObserverInterface * const, EventInterface * const );

    Grid( );
    ~Grid( );

	static void RefreshCache( );

	void ResetGrid ( );
    void FoodGrowth( );

	GridPoint GenerationStep( GridPoint const );

	PlannedActivity const GetPlan( ) const { return m_plan; };

	PERCENT      GetMutRate   ( GridPoint const gp ) { return getGridField( gp ).GetMutRate( ); }
	ENERGY_UNITS GetFertilizer( GridPoint const gp ) { return getGridField( gp ).GetFertilizer( ); }
	ENERGY_UNITS GetFoodStock ( GridPoint const gp ) { return getGridField( gp ).GetFoodStock( ); }
	ENERGY_UNITS GetFertility ( GridPoint const gp ) { return getGridField( gp ).GetFertility( ); }

	void Apply2MutRate   (GridPoint const gp, PERCENT      const s, ManipulatorFunc m) { getGridField( gp ).Apply2MutRate   (s, m); }
	void Apply2Fertilizer(GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField( gp ).Apply2Fertilizer(s, m); }
	void Apply2FoodStock (GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField( gp ).Apply2FoodStock (s, m); }
	void Apply2Fertility (GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField( gp ).Apply2Fertility (s, m); }

	void SetEnergy( GridPoint const gp, ENERGY_UNITS const s )         { getGridField( gp ).SetEnergy( s ); }
	void IncEnergy( GridPoint const gp, ENERGY_UNITS const s )         { getGridField( gp ).IncEnergy( s ); }
    void DecEnergy( GridPoint const gp, ENERGY_UNITS const s )         { getGridField( gp ).DecEnergy( s ); }

    void IncGenNr( ) { ++m_genEvo; }

    void EditSetStrategy( GridPoint const, PERCENT const, Strategy::Id );

    // GridPoint list operations

    void CheckGridPointList( ) const { m_gpList.CheckGridPointList( * this ); };

    GridPoint const GetOldestGp  ( ) const { return m_gpList.GetOldestGp  ( ); }
    GridPoint const GetYoungestGp( ) const { return m_gpList.GetYoungestGp( ); }

    GridPoint const GetSeniorGp( GridPoint const gp ) const { return GetGridField( gp ).GetSeniorGp( ); }
    GridPoint const GetJuniorGp( GridPoint const gp ) const { return GetGridField( gp ).GetJuniorGp( ); }

    void SetSeniorGp( GridPoint const gp, GridPoint const gpSenior ) { getGridField( gp ).SetSeniorGp( gpSenior ); }
    void SetJuniorGp( GridPoint const gp, GridPoint const gpJunior ) { getGridField( gp ).SetJuniorGp( gpJunior ); }

    bool ListIsEmpty( ) const { return m_gpList.ListIsEmpty( ); }

    GridPoint FindGridPoint( std::function<bool( GridPoint const)> const &, GridRect const & ) const;

    // Query functions 

    GridField const & GetGridField( GridPoint const gp ) const
    {
        assert( IsInGrid( gp ) );
        return m_aGF[ gp.GetXvalue() ][ gp.GetYvalue() ];
    };

    bool           IsAlive     ( GridPoint const gp ) const { return GetGridField( gp ).IsAlive( ); }
    bool           IsDead      ( GridPoint const gp ) const { return GetGridField( gp ).IsDead( ); }
    ENERGY_UNITS   GetFoodStock( GridPoint const gp ) const { return GetGridField( gp ).GetFoodStock( ); }
    IND_ID         GetId       ( GridPoint const gp ) const { return GetGridField( gp ).GetId       ( ); }
    tOrigin        GetOrigin   ( GridPoint const gp ) const { return GetGridField( gp ).GetOrigin   ( ); }
    EVO_GENERATION GetGenBirth ( GridPoint const gp ) const { return GetGridField( gp ).GetGenBirth( ); }
    EVO_GENERATION GetAge      ( GridPoint const gp ) const 
	{
		EVO_GENERATION genBirth = GetGenBirth( gp );
		return genBirth.IsNull() ? EVO_GENERATION::NULL_VAL() : (m_genEvo - genBirth); 
	}

	static BYTES GetGridExtraSize();

	EVO_GENERATION GetEvoGenerationNr( )       const { return m_genEvo; }
    int            GetNrOfLivingIndividuals( ) const { return m_gpList.GetSize( ); }

	void PrepareActionCounters( )
	{ 
		StrategyData::ResetCounters( );
		for ( auto & ax: ( m_ActionCounter ) )
			ax.fill( ACTION_COUNT(0) ); 
	}

	ACTION_COUNT GetActionCounter( Strategy::Id const strategy, Action::Id const action ) const 
	{
		unsigned int const uiAction   = static_cast<unsigned int>(action);
		unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

		assert( uiAction   < Action::COUNT );
		assert( uiStrategy < Strategy::COUNT );

		return m_ActionCounter[uiAction][uiStrategy];
	}

private:
		
	void incActionCounter( Strategy::Id const strategy, Action::Id const action )
	{
		unsigned int const uiAction   = static_cast<unsigned int>(action);
		unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

		assert( uiAction   < Action::COUNT );
		assert( uiStrategy < Strategy::COUNT );

		++ m_ActionCounter[uiAction][uiStrategy];
	}
		
    void deleteAndReset( GridField & gf )
    {
        m_gpList.DeleteGridPointFromList( * this, gf );
        gf.ResetIndividual( );  // zero individual data
    }

    void deleteIfDead( GridField & gf )
    {
        if ( gf.IsDead( ) )
            deleteAndReset( gf );
    }

    class getSlots;

    void getBestNeighborSlots( Neighborhood & );

    GridField & getGridField( GridPoint const gp )
    {
        assert( IsInGrid( gp ) );
        return m_aGF[ gp.GetXvalue() ][ gp.GetYvalue() ];
    };

	bool IsPoi ( GridPoint const gp ) 
	{ 
		return  gp.IsNotNull( ) &&  GridPOI::IsPoi( GetId(gp) ); 
	}

	GridPoint chooseTarget ( Neighborhood & );
    GridPoint choosePartner( Neighborhood & );


	GridPoint actionMove     ( GridField & );
	GridPoint actionClone    ( GridField & );
	GridPoint actionMarry    ( GridField & );
	GridPoint actionInteract ( GridField & );
	GridPoint actionPassOn   ( GridField & );
	GridPoint actionFertilize( GridField & );
	GridPoint actionEat      ( GridField & );
	GridPoint actionUndefined( GridField & );

	void stopOnPoi( GridField const & );

	// member variables

	vector< vector < GridField > > m_aGF;                  // 15.000 * 108 byte = 1.620.000 byte
                                                        
    GridPointList   m_gpList;                              //                            10 byte
    EVO_GENERATION  m_genEvo;                              //                             4 byte
    Neighborhood    m_emptyNeighborSlots;
    Neighborhood    m_occupiedNeighborSlots;
	PlannedActivity m_plan;                                

	array< array < ACTION_COUNT, Strategy::COUNT>, Action::COUNT > m_ActionCounter;

    // following members are stored here only to be part of grid history.

    Random m_random;                                               //  16 byte
    IND_ID m_idCounter;                                            //   4 byte
     
    // static members

	static ObserverInterface * m_pObservers;    // GUI call back for display of current model 
	static EventInterface    * m_pEventPOI;

    static GROWTH_RATE  m_enFoodGrowthRate;
    static ENERGY_UNITS m_enBasicFoodConsumption;
    static ENERGY_UNITS m_enMemSizeFoodConsumption;
    static ENERGY_UNITS m_enMoveFoodConsumption;
    static ENERGY_UNITS m_enCloneFoodConsumption;
    static ENERGY_UNITS m_enMarryFoodConsumption;
    static ENERGY_UNITS m_enInteractFoodConsumption;
    static bool         m_bNeighborhoodFoodSensitivity;
};

void CheckIndividuals( Grid & );
