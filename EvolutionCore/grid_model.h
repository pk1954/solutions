// grid_model.h : 
//

#pragma once

#include "random.h"
#include "gridPoint.h"
#include "gpList.h"
#include "gridField.h"
#include "gridNeighbor.h"
#include "EvolutionTypes.h"
#include "plannedActivity.h"
#include "Manipulator.h"

class GridCircle;
class GridRect;

class Grid
{
public:

    Grid( );
    ~Grid( );

    void      ResetGrid    ( );
    void      FoodGrowth   ( );
    void      MakePlan     ( GridPoint const &, PlannedActivity & );
    GridPoint ImplementPlan( GridPoint const &, PlannedActivity const & );

	short GetFertilizer  ( GridPoint const & gp ) { return getGridField( gp ).GetFertilizer( ); }
	short GetFoodStock   ( GridPoint const & gp ) { return getGridField( gp ).GetFoodStock( ); }
	short GetFertility   ( GridPoint const & gp ) { return getGridField( gp ).GetFertility( ); }
	short GetMutationRate( GridPoint const & gp ) { return getGridField( gp ).GetMutationRate( ); }

	void Apply2Fertilizer(GridPoint const & gp, short const s, Manipulator<short> * man) { getGridField( gp ).Apply2Fertilizer(s, man); }
	void Apply2FoodStock (GridPoint const & gp, short const s, Manipulator<short> * man) { getGridField( gp ).Apply2FoodStock (s, man); }
	void Apply2Fertility (GridPoint const & gp, short const s, Manipulator<short> * man) { getGridField( gp ).Apply2Fertility (s, man); }
	void Apply2MutRate   (GridPoint const & gp, short const s, Manipulator<short> * man) { getGridField( gp ).Apply2MutRate   (s, man); }

	void SetEnergy( GridPoint const & gp, short const s ) { getGridField( gp ).SetEnergy( s ); }
	void IncEnergy( GridPoint const & gp, short const s ) { getGridField( gp ).IncEnergy( s ); }
    void DecEnergy( GridPoint const & gp, short const s ) { getGridField( gp ).DecEnergy( s ); }

    void IncGenNr( ) { ++m_genEvo; }

    void EditSetStrategy( GridPoint const &, unsigned int const, Manipulator<short> *, tStrategyId );

    unsigned int NextRandomNumber      ( void ) { return m_random.NextRandomNumber( ); };
    double       Distribution( void ) { return m_random.NextWeightedDistribution( ); };

    // GridPoint list operations

    void CheckGridPointList( ) const { m_gpList.CheckGridPointList( * this ); };

    GridPoint const & GetOldestGp  ( ) const { return m_gpList.GetOldestGp  ( ); }
    GridPoint const & GetYoungestGp( ) const { return m_gpList.GetYoungestGp( ); }

    GridPoint const & GetSeniorGp( GridPoint const & gp ) const { return GetGridField( gp ).GetSeniorGp( ); }
    GridPoint const & GetJuniorGp( GridPoint const & gp ) const { return GetGridField( gp ).GetJuniorGp( ); }

    void SetSeniorGp( GridPoint const & gp, GridPoint const & gpSenior ) { getGridField( gp ).SetSeniorGp( gpSenior ); }
    void SetJuniorGp( GridPoint const & gp, GridPoint const & gpJunior ) { getGridField( gp ).SetJuniorGp( gpJunior ); }

    bool ListIsEmpty( ) const { return m_gpList.ListIsEmpty( ); }

    GridPoint FindGridPoint( const std::function<bool( GridPoint const &)>& ) const;

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

    // static functions

    static void InitClass( );

private:
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

    GridField      m_aGF[ GridPoint::GRID_WIDTH ][ GridPoint::GRID_HEIGHT ];   // 20.000 * 156 byte = 3.120.000 byte
    GridPointList  m_gpList;                                               //                            10 byte
    long           m_lFoodGrowth;    // for statistics                     //                             8 byte 
    EVO_GENERATION m_genEvo;                                               //                             4 byte
    Neighborhood   m_emptyNeighborSlots;
    Neighborhood   m_occupiedNeighborSlots;

    // following members must only be stored here to be part of grid history.

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
