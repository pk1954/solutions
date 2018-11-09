// grid_model.cpp :
//

#include "stdafx.h"
#include "assert.h"
#include <cstdlib> 
#include <cmath> 
#include "strategy.h"
#include "individual.h"
#include "interaction.h"
#include "gridNeighbor.h"
#include "grid_model.h"
#include "gplIterator.h"
#include "gpList.h"
#include "config.h"
#include "dump.h"
#include "genome.h"
#include "gridField.h"
#include "gridRect.h"

using namespace std;

//#ifndef NDEBUG
//    #define CHECK_INDIVIDUALS CheckIndividuals( * this )
//#else
    #define CHECK_INDIVIDUALS
//#endif

int  Grid::m_iFoodGrowthRate;
int  Grid::m_iBasicFoodConsumption;
int  Grid::m_iMemSizeFoodConsumption;
int  Grid::m_iMoveFoodConsumption;
int  Grid::m_iCloneFoodConsumption;
int  Grid::m_iMarryFoodConsumption;
int  Grid::m_iInteractFoodConsumption;
bool Grid::m_bNeighborhoodFoodSensitivity;

void Grid::InitClass( )
{
    INTERACTION::InitClass( );
    Genome::InitClass( );
    GridField::InitClass( );
    Individual::InitClass( );
    Neighborhood::InitClass( Config::GetConfigValue( Config::tId::nrOfNeighbors ) );

    m_iFoodGrowthRate              = Config::GetConfigValue( Config::tId::growthRateFood );
    m_iBasicFoodConsumption        = Config::GetConfigValue( Config::tId::energyConsumptionBasicRate );
    m_iMemSizeFoodConsumption      = Config::GetConfigValue( Config::tId::energyConsumptionMemSize );
    m_iMoveFoodConsumption         = Config::GetConfigValue( Config::tId::energyConsumptionMove );
    m_iCloneFoodConsumption        = Config::GetConfigValue( Config::tId::energyConsumptionClone );
    m_iMarryFoodConsumption        = Config::GetConfigValue( Config::tId::energyConsumptionMarry );
    m_iInteractFoodConsumption     = Config::GetConfigValue( Config::tId::energyConsumptionInteraction );
    m_bNeighborhoodFoodSensitivity = Config::GetConfigValue( Config::tId::neighborhoodFoodSensitivity ) > 0;
}

Grid::Grid( )
    : m_gpList( ),
      m_lFoodGrowth( 0 ),
      m_random( ),
      m_idCounter( ),
      m_genEvo( 0L ),
	  m_emptyNeighborSlots( ),
	  m_occupiedNeighborSlots( ),
	  m_pActionCounterFill( & m_ActionCounter1 ),
	  m_pActionCounterRead( & m_ActionCounter2 )
{
    Apply2Grid    // initialization of grid variables which never change after initialization
	( 
    	[&](GridPoint const gp)
		{
           getGridField( gp ).InitGridFieldStructure( gp );
		}
	); 
}

Grid::~Grid( )
{
    try
    {
        m_gpList.ResetGpList( );
    }
    catch ( ... )
    {
        exit( 1 );
    };
}

void CheckIndividuals( Grid & grid )
{
    int iCount = 0;
 
	Apply2Grid
	( 
    	[&](GridPoint const gp)
		{
            if ( grid.IsAlive( gp ) )
               ++ iCount;
		}
	);

	int const iNrOfLivingIndividuals = grid.GetNrOfLivingIndividuals( );
    assert( iCount == iNrOfLivingIndividuals );
}

void Grid::ResetGrid( )
{
    short sFood = Config::GetConfigValueShort( Config::tId::minFood );

    Apply2Grid
	( 
    	[&](GridPoint const gp)
		{
			getGridField( gp ).ResetGridField( sFood );
		}
	);

    m_gpList.ResetGpList( );
    m_random.InitializeRandom( );
}

GridPoint Grid::chooseTarget( Neighborhood & gpListEmpty )
{
    GridPoint m_gpTarget;

    if ( m_bNeighborhoodFoodSensitivity )
        getBestNeighborSlots( gpListEmpty );   // consider only neighbor slots with best food stock

    unsigned int const uiRandom = m_random.NextRandomNumber( );
    m_gpTarget = gpListEmpty.GetRandomElement( uiRandom ); // choose one of them at random
    assert( IsDead( m_gpTarget ) );

    return m_gpTarget;
}

GridPoint Grid::choosePartner( Neighborhood & gpListFilled )
{
    GridPoint m_gpPartner;

    unsigned int const uiRandom = m_random.NextRandomNumber( );
    m_gpPartner = gpListFilled.GetRandomElement( uiRandom );
    assert( IsAlive( m_gpPartner ) );

    return m_gpPartner;
}

void Grid::MakePlan
( 
    GridPoint       const gpRun, 
    PlannedActivity     & plan
)
{
    GridField const & gfRun = GetGridField( gpRun );
    assert( gfRun.IsAlive( ) );

    plan.SetActor( gpRun );

    // basic food consumption

    plan.SetBaseConsumption( m_iBasicFoodConsumption );
    plan.IncBaseConsumption( m_iMemSizeFoodConsumption * gfRun.GetMemSize( ) );

	m_emptyNeighborSlots.ClearList( );
	m_occupiedNeighborSlots.ClearList( );
    Neighborhood::Apply2All
	( 
		gpRun, 
		[&](GridPoint const gpNeighbor)
		{
			(IsAlive(gpNeighbor) ? &m_occupiedNeighborSlots : &m_emptyNeighborSlots)->AddToList( gpNeighbor );
		}
	);

    assert( m_emptyNeighborSlots.GetLength() + m_occupiedNeighborSlots.GetLength() == Neighborhood::GetNrOfNeighbors( ) );

    tAction action = gfRun.GetGenome( ).GetOption
	( 
        m_emptyNeighborSlots.GetLength( ) > 0,            // has free space around? 
        m_occupiedNeighborSlots.GetLength( ) > 0,         // has neighbor
        gfRun.GetEnergy( ) - plan.GetBaseConsumption( ),  // available energy
	    GetAge( gpRun ),
		m_random 
	);

    plan.SetActionType( action );

    switch ( action )
    {
    case tAction::move:	// choose a free neighbour cell and move there
    case tAction::clone:  // choose a free neighbour cell and create a clone there 
        plan.SetTarget ( chooseTarget( m_emptyNeighborSlots ) );
        plan.NoPartner( );
        break;

    case tAction::marry:     // choose a living neighbour and marry
        plan.SetPartner( choosePartner( m_occupiedNeighborSlots ) );
        plan.SetTarget ( chooseTarget ( m_emptyNeighborSlots ) );
        break;

    case tAction::interact:  // choose a living neighbour and interact
        plan.SetPartner( choosePartner( m_occupiedNeighborSlots ) );
        plan.NoTarget( );
        break;

    case tAction::passOn:
    case tAction::fertilize:
    case tAction::eat:
        plan.NoPartner( );
        plan.NoTarget ( );
        break;

    case tAction::undefined:
        break;

	default:
        assert( false );
    }
}

GridPoint Grid::ImplementPlan   // may return GP_NULL
( 
    GridPoint       const   gpRun, 
    PlannedActivity const & plan
)  
{
    GridField & gfRun  = getGridField( gpRun );
    GridPoint   gpNext = gfRun.GetJuniorGp( );      // may be GP_NULL

    gfRun.SetLastAction( plan.GetActionType( ) );
    gfRun.DecEnergy    ( plan.GetBaseConsumption( ) );

	incActionCounter( plan.GetActionType( ), gfRun.GetStrategyId( ) );

	switch ( plan.GetActionType( ) )
    {
        case tAction::move: 
        {
            gfRun.DecEnergy( m_iMoveFoodConsumption );
            if ( gfRun.IsAlive( ) )
            {
                GridField & gfTarget = getGridField( plan.GetTarget( ) );
                m_gpList.ReplaceGridPointInList( * this, gfRun, gfTarget ); 
                gfTarget.MoveIndividual( gfRun );
			    return gpNext;                    // avoid "deleteIfDead( gfRun );" after switch!
            }
        }
        break;

        case tAction::clone:
        {
            GridField & gfTarget = getGridField( plan.GetTarget( ) );

            gfRun.DecEnergy( m_iCloneFoodConsumption );
            gfTarget.CloneIndividual( ++m_idCounter, m_genEvo, m_random, gfRun );

            if ( gfTarget.IsAlive( ) )
                m_gpList.AddGridPointToList( *this, gfTarget );
        }
        break;

        case tAction::marry:
        {
            GridField & gfTarget  = getGridField( plan.GetTarget( ) );
            GridField & gfPartner = getGridField( plan.GetPartner( ) );

            gfRun.DecEnergy( m_iMarryFoodConsumption );
            gfTarget.BreedIndividual( ++m_idCounter, m_genEvo, m_random, gfRun, gfPartner );

            if ( gfTarget.IsAlive( ) )
                m_gpList.AddGridPointToList( *this, gfTarget );

            deleteIfDead( gfPartner );
        }
        break;

        case tAction::interact:
        {
            GridField & gfPartner = getGridField( plan.GetPartner( ) );
            gfRun.DecEnergy( m_iInteractFoodConsumption );
            GridField::Interact( gfRun, gfPartner );

            if ( gpNext.IsNotNull( ) )
            {
                GridField & gfNext = getGridField( gpNext );
                if ( gfNext.IsDead( ) )
                    gpNext = gfNext.GetJuniorGp( );
            }

            deleteIfDead( gfPartner );
        }
        break;

        case tAction::fertilize:
        {
            short const sInvest = gfRun.GetAllele( tGeneType::fertilInvest );
            gfRun.Fertilize( sInvest );
            gfRun.DecEnergy( sInvest );
        }
        break;

        case tAction::passOn:  
        {
            gfRun.PassOn2Child( ++m_idCounter, m_genEvo, m_random );
        }
        break;

        case tAction::eat:  
        {
            short const sWant    = gfRun.GetAllele( tGeneType::appetite );
            short const sReceive = gfRun.GetConsumption( sWant );
            gfRun.DecFoodStock( sReceive );
            gfRun.IncEnergy   ( sReceive );
        }
        break;

	    case tAction::undefined:
        break;

        default:
            assert( false );
    }

	deleteIfDead( gfRun );

    CHECK_INDIVIDUALS;
    return gpNext;
}

void Grid::EditSetStrategy
( 
    GridPoint   const gp, 
    short       const sIntensity, // percent value
    tStrategyId       strategy
)
{
	assert( sIntensity >= 0 );
	if ( m_random.NextRandomNumber( ) < static_cast<unsigned short>(sIntensity) * Random::MAX_VAL / 100ul )
	{
		GridField & gf       = getGridField( gp );
		bool const  bIsAlive = gf.IsAlive();

		if ( strategy != tStrategyId::empty )
		{
			if ( strategy == tStrategyId::random ) 
				strategy = static_cast<tStrategyId>(m_random.NextRandomNumber() % NR_STRATEGIES );

			gf.CreateIndividual( ++m_idCounter, m_genEvo, strategy );
			if ( ! bIsAlive )
				m_gpList.AddGridPointToList( * this, gf );
		}
		else if ( bIsAlive )
		{
			deleteAndReset( gf );
		}
	}
}

GridPoint Grid::FindGridPoint
(
	const std::function<bool( GridPoint const)>& func, 
	GridRect const & rect
) const
{
    GridPoint gp;
    for ( gp.y = rect.GetStartPoint().y; gp.y <= rect.GetEndPoint().y; ++gp.y )
    for ( gp.x = rect.GetStartPoint().x; gp.x <= rect.GetEndPoint().x; ++gp.x )
        if ( func( gp ) )
        {
            return gp;
        }
    return GridPoint::GP_NULL;
}
