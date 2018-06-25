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

    void IncFertilizer  ( GridPoint const & gp, short const sInc ) { getGridField( gp ).IncFertilizer  ( sInc ); }
    void IncFoodStock   ( GridPoint const & gp, short const sInc ) { getGridField( gp ).IncFoodStock   ( sInc ); }
    void IncFertility   ( GridPoint const & gp, short const sInc ) { getGridField( gp ).IncFertility   ( sInc ); }
    void IncMutationRate( GridPoint const & gp, short const sInc ) { getGridField( gp ).IncMutationRate( sInc ); }
    void IncEnergy      ( GridPoint const & gp, short const sInc ) { getGridField( gp ).IncEnergy      ( sInc ); }
    void DecEnergy      ( GridPoint const & gp, short const sDec ) { getGridField( gp ).DecEnergy      ( sDec ); }

    void IncGenNr( ) { ++m_genEvo; }

    void EditSetStrategy( GridPoint const &, unsigned int const, tStrategyId const );

    unsigned int NextRandomNumber      ( void ) { return m_random.NextRandomNumber( ); };
    double       NextNormalDistribution( void ) { return m_random.NextNormalDistribution( ); };

    // GridPoint list operations

    void CheckGridPointList( ) const { m_gpList.CheckGridPointList( * this ); };

    GridPoint const & GetOldestGp  ( ) const { return m_gpList.GetOldestGp  ( ); }
    GridPoint const & GetYoungestGp( ) const { return m_gpList.GetYoungestGp( ); }

    GridPoint const & GetSeniorGp( GridPoint const & gp ) const { return GetGridField( gp ).GetSeniorGp( ); }
    GridPoint const & GetJuniorGp( GridPoint const & gp ) const { return GetGridField( gp ).GetJuniorGp( ); }

    void SetSeniorGp( GridPoint const & gp, GridPoint const & gpSenior ) { getGridField( gp ).SetSeniorGp( gpSenior ); }
    void SetJuniorGp( GridPoint const & gp, GridPoint const & gpJunior ) { getGridField( gp ).SetJuniorGp( gpJunior ); }

    bool ListIsEmpty( ) const { return m_gpList.ListIsEmpty( ); }

    GridPoint FindGridPoint(  const std::function<bool( GridPoint const &)>& ) const;

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

    unsigned int GetAverageFoodGrowth    ( ) const { return m_uiFoodGrowth / GridPoint::GRID_AREA; }
    int          GetNrOfLivingIndividuals( ) const { return m_gpList.GetSize( ); }

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
    unsigned int   m_uiFoodGrowth;   // for statistics                     //                             8 byte 
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
