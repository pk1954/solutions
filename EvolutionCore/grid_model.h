// grid_model.h : 
//

#pragma once

#include <array> 
#include <algorithm>
#include "random.h"
#include "gridPoint.h"
#include "gridRect.h"
#include "gpList.h"
#include "gridField.h"
#include "gridNeighbor.h"
#include "EvolutionTypes.h"
#include "plannedActivity.h"

class GridCircle;
class GridRect;
class Manipulator;

class Grid
{
public:

    Grid( );
    ~Grid( );

    void      ResetGrid    ( );
    void      FoodGrowth   ( );
    void      MakePlan     ( GridPoint const &, PlannedActivity & );
    GridPoint ImplementPlan( GridPoint const &, PlannedActivity const & );

	short GetFertilizer( GridPoint const & gp ) { return getGridField( gp ).GetFertilizer( ); }
	short GetFoodStock ( GridPoint const & gp ) { return getGridField( gp ).GetFoodStock( ); }
	short GetFertility ( GridPoint const & gp ) { return getGridField( gp ).GetFertility( ); }
	short GetMutRate   ( GridPoint const & gp ) { return getGridField( gp ).GetMutRate( ); }

	void Apply2Fertilizer(GridPoint const & gp, short const s, ManipulatorFunc m) { getGridField( gp ).Apply2Fertilizer(s, m); }
	void Apply2FoodStock (GridPoint const & gp, short const s, ManipulatorFunc m) { getGridField( gp ).Apply2FoodStock (s, m); }
	void Apply2Fertility (GridPoint const & gp, short const s, ManipulatorFunc m) { getGridField( gp ).Apply2Fertility (s, m); }
	void Apply2MutRate   (GridPoint const & gp, short const s, ManipulatorFunc m) { getGridField( gp ).Apply2MutRate   (s, m); }

	void SetEnergy( GridPoint const & gp, short const s ) { getGridField( gp ).SetEnergy( s ); }
	void IncEnergy( GridPoint const & gp, short const s ) { getGridField( gp ).IncEnergy( s ); }
    void DecEnergy( GridPoint const & gp, short const s ) { getGridField( gp ).DecEnergy( s ); }

    void IncGenNr( ) { ++m_genEvo; }

    void EditSetStrategy( GridPoint const &, short const, tStrategyId );

    // GridPoint list operations

    void CheckGridPointList( ) const { m_gpList.CheckGridPointList( * this ); };

    GridPoint const & GetOldestGp  ( ) const { return m_gpList.GetOldestGp  ( ); }
    GridPoint const & GetYoungestGp( ) const { return m_gpList.GetYoungestGp( ); }

    GridPoint const & GetSeniorGp( GridPoint const & gp ) const { return GetGridField( gp ).GetSeniorGp( ); }
    GridPoint const & GetJuniorGp( GridPoint const & gp ) const { return GetGridField( gp ).GetJuniorGp( ); }

    void SetSeniorGp( GridPoint const & gp, GridPoint const & gpSenior ) { getGridField( gp ).SetSeniorGp( gpSenior ); }
    void SetJuniorGp( GridPoint const & gp, GridPoint const & gpJunior ) { getGridField( gp ).SetJuniorGp( gpJunior ); }

    bool ListIsEmpty( ) const { return m_gpList.ListIsEmpty( ); }

    GridPoint FindGridPoint( const std::function<bool( GridPoint const &)>&, GridRect const & = GridRect::GRID_RECT_FULL ) const;

    // Query functions 

    GridField const & GetGridField( GridPoint const & gp ) const
    {
        assert( gp.IsInGrid( ) );
        return m_aGF[ gp.x ][ gp.y ];
    };

    bool           IsAlive     ( GridPoint const & gp ) const { return GetGridField( gp ).IsAlive( ); }
    bool           IsDead      ( GridPoint const & gp ) const { return GetGridField( gp ).IsDead( ); }
    int            GetFoodStock( GridPoint const & gp ) const { return GetGridField( gp ).GetFoodStock( ); }
    IndId          GetId       ( GridPoint const & gp ) const { return GetGridField( gp ).GetId       ( ); }
    tOrigin        GetOrigin   ( GridPoint const & gp ) const { return GetGridField( gp ).GetOrigin   ( ); }
    EVO_GENERATION GetGenBirth ( GridPoint const & gp ) const { return GetGridField( gp ).GetGenBirth( ); }
    EVO_GENERATION GetAge      ( GridPoint const & gp ) const { return m_genEvo - GetGenBirth( gp ); }

    EVO_GENERATION GetEvoGenerationNr( ) const { return m_genEvo; }

    long GetAverageFoodGrowth    ( ) const { return m_lFoodGrowth / GridPoint::GRID_AREA; }
    int  GetNrOfLivingIndividuals( ) const { return m_gpList.GetSize( ); }

	void PrepareActionCounters( )
	{ 
		std::swap( m_pActionCounterFill, m_pActionCounterRead );
		for ( auto & ax: ( * m_pActionCounterFill ) )
			ax.fill( 0 ); 
	}

	unsigned int GetActionCounter
	( 
		unsigned int const uiStrategy, 
		tAction      const action
	) const
	{
		unsigned int uiAction = static_cast<unsigned int>( action );
		assert( uiAction   <= NR_ACTIONS );
		assert( uiStrategy <= NR_STRATEGIES );
		return (* m_pActionCounterRead)[ uiAction ][ uiStrategy ];
	}

    // static functions

    static void InitClass( );

private:
	void incActionCounter
	(
		tAction     action,
		tStrategyId strategy
	)
	{
		if ( action != tAction::undefined )
		{
			unsigned int const uiAction   = static_cast<unsigned int>(action);
			unsigned int const uiStrategy = static_cast<unsigned int>(strategy);
			++ ( * m_pActionCounterFill )[uiAction][uiStrategy];
		}
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

    GridField & getGridField( GridPoint const & gp )
    {
        assert( gp.IsInGrid( ) );
        return m_aGF[ gp.x ][ gp.y ];
    };

    GridField const & getGridFieldC( GridPoint const & gp ) const
    {
        assert( gp.IsInGrid( ) );
        return m_aGF[ gp.x ][ gp.y ];
    };

    GridPoint chooseTarget ( Neighborhood & );
    GridPoint choosePartner( Neighborhood & );

    // member variables

    GridField      m_aGF[ GridPoint::GRID_WIDTH ][ GridPoint::GRID_HEIGHT ];   // 20.000 * 196 byte = 3.920.000 byte
    GridPointList  m_gpList;                                               //                                10 byte
    long           m_lFoodGrowth;    // for statistics                     //                                 8 byte 
    EVO_GENERATION m_genEvo;                                               //                                 4 byte
    Neighborhood   m_emptyNeighborSlots;
    Neighborhood   m_occupiedNeighborSlots;

	typedef array< array < unsigned int, NR_STRATEGIES>, NR_ACTIONS > tActionCounters;
	tActionCounters   m_ActionCounter1;
	tActionCounters   m_ActionCounter2;
	tActionCounters * m_pActionCounterFill;
	tActionCounters * m_pActionCounterRead;

    // following members are stored here only to be part of grid history.

    Random         m_random;                                               //                            16 byte
    IndId          m_idCounter;  // Used only by class Individual.         //                             4 byte
     
    // static members 

    static int  m_iFoodGrowthRate;
    static int  m_iBasicFoodConsumption;
    static int  m_iMemSizeFoodConsumption;
    static int  m_iMoveFoodConsumption;
    static int  m_iCloneFoodConsumption;
    static int  m_iMarryFoodConsumption;
    static int  m_iInteractFoodConsumption;
    static bool m_bNeighborhoodFoodSensitivity;
};

void CheckIndividuals( Grid & );
