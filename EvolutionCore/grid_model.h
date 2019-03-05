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
class Manipulator;

using GROWTH_RATE = NamedType< unsigned int, struct GROWTH_RATET_Parameter >;

class Grid
{
public:

    Grid( );
    ~Grid( );

    void      ResetGrid    ( );
    void      FoodGrowth   ( );
    void      MakePlan     ( GridPoint const, PlannedActivity & );
    GridPoint ImplementPlan( GridPoint const, PlannedActivity const & );

	short        GetMutRate   ( GridPoint const gp ) { return getGridField( gp ).GetMutRate( ); }
	ENERGY_UNITS GetFertilizer( GridPoint const gp ) { return getGridField( gp ).GetFertilizer( ); }
	ENERGY_UNITS GetFoodStock ( GridPoint const gp ) { return getGridField( gp ).GetFoodStock( ); }
	ENERGY_UNITS GetFertility ( GridPoint const gp ) { return getGridField( gp ).GetFertility( ); }

	void Apply2MutRate   (GridPoint const gp, short        const s, ManipulatorFunc m) { getGridField( gp ).Apply2MutRate   (s, m); }
	void Apply2Fertilizer(GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField( gp ).Apply2Fertilizer(s, m); }
	void Apply2FoodStock (GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField( gp ).Apply2FoodStock (s, m); }
	void Apply2Fertility (GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField( gp ).Apply2Fertility (s, m); }

	void SetEnergy( GridPoint const gp, ENERGY_UNITS const s ) { getGridField( gp ).SetEnergy( s ); }
	void IncEnergy( GridPoint const gp, ENERGY_UNITS const s ) { getGridField( gp ).IncEnergy( s ); }
    void DecEnergy( GridPoint const gp, ENERGY_UNITS const s ) { getGridField( gp ).DecEnergy( s ); }

    void IncGenNr( ) { ++m_genEvo; }

    void EditSetStrategy( GridPoint const, short const, Strategy::Id );

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

    EVO_GENERATION GetEvoGenerationNr( ) const { return m_genEvo; }

    ENERGY_UNITS GetAverageFoodGrowth    ( ) const { return ENERGY_UNITS(m_enFoodGrowth / GRID_AREA()); }
    int          GetNrOfLivingIndividuals( ) const { return m_gpList.GetSize( ); }

	void PrepareActionCounters( )
	{ 
		std::swap( m_pActionCounterFill, m_pActionCounterRead );
		for ( auto & ax: ( * m_pActionCounterFill ) )
			ax.fill( ACTION_COUNT(0) ); 
	}

	ACTION_COUNT GetActionCounter( Strategy::Id const strategy, Action::Id const action ) const 
	{
		return actionCounter( strategy, action );
	}

    // static functions

    static void InitClass( );

private:
	ACTION_COUNT & actionCounter( Strategy::Id const strategy, Action::Id const action ) const
	{
		unsigned int const uiAction   = static_cast<unsigned int>(action);
		unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

		assert( uiAction   <= Action::NR_ACTIONS );
		assert( uiStrategy <= Strategy::COUNT );

		return ( * m_pActionCounterFill )[uiAction][uiStrategy];
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

    GridField const & getGridFieldC( GridPoint const gp ) const
    {
        assert( IsInGrid( gp ) );
        return m_aGF[ gp.GetXvalue() ][ gp.GetYvalue() ];
    };

    GridPoint chooseTarget ( Neighborhood & );
    GridPoint choosePartner( Neighborhood & );

    // member variables

    GridField      m_aGF[ GRID_WIDTH_VAL ][ GRID_HEIGHT_VAL ];   // 20.000 * 196 byte = 3.920.000 byte
    GridPointList  m_gpList;                               //                            10 byte
    ENERGY_UNITS   m_enFoodGrowth;    // for statistics    //                             8 byte 
    EVO_GENERATION m_genEvo;                               //                             4 byte
    Neighborhood   m_emptyNeighborSlots;
    Neighborhood   m_occupiedNeighborSlots;

	using tActionCounters = std::array< std::array < ACTION_COUNT, Strategy::COUNT>, Action::NR_ACTIONS >;

	tActionCounters   m_ActionCounter1;
	tActionCounters   m_ActionCounter2;
	tActionCounters * m_pActionCounterFill;
	tActionCounters * m_pActionCounterRead;

    // following members are stored here only to be part of grid history.

    Random m_random;                                               //  16 byte
    IND_ID m_idCounter;  // Used only by class Individual.         //   4 byte
     
    // static members 

    static GROWTH_RATE  m_iFoodGrowthRate;
    static ENERGY_UNITS m_iBasicFoodConsumption;
    static ENERGY_UNITS m_iMemSizeFoodConsumption;
    static ENERGY_UNITS m_iMoveFoodConsumption;
    static ENERGY_UNITS m_iCloneFoodConsumption;
    static ENERGY_UNITS m_iMarryFoodConsumption;
    static ENERGY_UNITS m_iInteractFoodConsumption;
    static bool         m_bNeighborhoodFoodSensitivity;
};

void CheckIndividuals( Grid & );
