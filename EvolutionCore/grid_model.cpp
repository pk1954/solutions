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

//#ifndef NDEBUG
//    #define CHECK_INDIVIDUALS CheckIndividuals( * this )
//#else
    #define CHECK_INDIVIDUALS
//#endif

int          Grid::m_iFoodGrowthRate;
ENERGY_UNITS Grid::m_iBasicFoodConsumption;
ENERGY_UNITS Grid::m_iMemSizeFoodConsumption;
ENERGY_UNITS Grid::m_iMoveFoodConsumption;
ENERGY_UNITS Grid::m_iCloneFoodConsumption;
ENERGY_UNITS Grid::m_iMarryFoodConsumption;
ENERGY_UNITS Grid::m_iInteractFoodConsumption;
bool Grid::m_bNeighborhoodFoodSensitivity;

void Grid::InitClass( )
{
    INTERACTION::InitClass( );
    Genome::InitClass( );
    GridField::InitClass( );
    Individual::InitClass( );
    Neighborhood::InitClass( Config::GetConfigValue( Config::tId::nrOfNeighbors ) );

    m_bNeighborhoodFoodSensitivity = Config::GetConfigValue( Config::tId::neighborhoodFoodSensitivity ) > 0;
    m_iFoodGrowthRate              = Config::GetConfigValue( Config::tId::growthRateFood );

	m_iBasicFoodConsumption        = ENERGY_UNITS(Config::GetConfigValue( Config::tId::energyConsumptionBasicRate ));
    m_iMemSizeFoodConsumption      = ENERGY_UNITS(Config::GetConfigValue( Config::tId::energyConsumptionMemSize ));
    m_iMoveFoodConsumption         = ENERGY_UNITS(Config::GetConfigValue( Config::tId::energyConsumptionMove ));
    m_iCloneFoodConsumption        = ENERGY_UNITS(Config::GetConfigValue( Config::tId::energyConsumptionClone ));
    m_iMarryFoodConsumption        = ENERGY_UNITS(Config::GetConfigValue( Config::tId::energyConsumptionMarry ));
    m_iInteractFoodConsumption     = ENERGY_UNITS(Config::GetConfigValue( Config::tId::energyConsumptionInteraction ));
}

Grid::Grid( )
    : m_gpList( ),
      m_enFoodGrowth( ENERGY_UNITS(0) ),
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
    ENERGY_UNITS enFood = ENERGY_UNITS( Config::GetConfigValueShort( Config::tId::minFood ) );

    Apply2Grid
	( 
    	[&](GridPoint const gp)
		{
			getGridField( gp ).ResetGridField( enFood );
		}
	);

    m_gpList.ResetGpList( );
    m_random.InitializeRandom( );
}

GridPoint Grid::chooseTarget( Neighborhood & gpListEmpty )
{
    if ( m_bNeighborhoodFoodSensitivity )
        getBestNeighborSlots( gpListEmpty );   // consider only neighbor slots with best food stock

    GridPoint m_gpTarget = gpListEmpty.GetRandomElement( m_random.NextRandomNumber( ) ); // choose one of them at random
    assert( IsDead( m_gpTarget ) );
    return m_gpTarget;
}

GridPoint Grid::choosePartner( Neighborhood & gpListFilled )
{
    GridPoint const m_gpPartner = gpListFilled.GetRandomElement( m_random.NextRandomNumber( ) );
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
    plan.IncBaseConsumption( m_iMemSizeFoodConsumption * gfRun.GetMemSize( ).GetValue() );

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

    Action::Id action = gfRun.GetGenome( ).GetOption
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
    case Action::Id::move:	// choose a free neighbour cell and move there
    case Action::Id::clone:  // choose a free neighbour cell and create a clone there 
        plan.SetTarget ( chooseTarget( m_emptyNeighborSlots ) );
        plan.NoPartner( );
        break;

    case Action::Id::marry:     // choose a living neighbour and marry
        plan.SetPartner( choosePartner( m_occupiedNeighborSlots ) );
        plan.SetTarget ( chooseTarget ( m_emptyNeighborSlots ) );
        break;

    case Action::Id::interact:  // choose a living neighbour and interact
        plan.SetPartner( choosePartner( m_occupiedNeighborSlots ) );
        plan.NoTarget( );
        break;

    case Action::Id::passOn:
    case Action::Id::fertilize:
    case Action::Id::eat:
        plan.NoPartner( );
        plan.NoTarget ( );
        break;

    case Action::Id::undefined:
        break;

	default:
        assert( false );
    }
}

GridPoint Grid::ImplementPlan   // may return NULL_VAL
( 
    GridPoint       const   gpRun, 
    PlannedActivity const & plan
)  
{
    GridField & gfRun  = getGridField( gpRun );
    GridPoint   gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL

    gfRun.SetLastAction( plan.GetActionType( ) );
    gfRun.DecEnergy    ( plan.GetBaseConsumption( ) );

	incActionCounter( gfRun.GetStrategyId( ), plan.GetActionType( ) );

	switch ( plan.GetActionType( ) )
    {
        case Action::Id::move: 
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

        case Action::Id::clone:
        {
            GridField & gfTarget = getGridField( plan.GetTarget( ) );

            gfRun.DecEnergy( m_iCloneFoodConsumption );
            gfTarget.CloneIndividual( ++m_idCounter, m_genEvo, m_random, gfRun );

            if ( gfTarget.IsAlive( ) )
                m_gpList.AddGridPointToList( *this, gfTarget );
        }
        break;

        case Action::Id::marry:
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

        case Action::Id::interact:
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

        case Action::Id::fertilize:
        {
            ENERGY_UNITS const enInvest = ENERGY_UNITS(gfRun.GetAllele( GeneType::Id::fertilInvest ));
            gfRun.Fertilize( enInvest );
            gfRun.DecEnergy( enInvest );
        }
        break;

        case Action::Id::passOn:  
        {
            gfRun.PassOn2Child( ++m_idCounter, m_genEvo, m_random );
        }
        break;

        case Action::Id::eat:  
        {
            ENERGY_UNITS const enWant    = ENERGY_UNITS( gfRun.GetAllele( GeneType::Id::appetite ) );
            ENERGY_UNITS const enReceive = gfRun.GetConsumption( enWant );
            gfRun.IncFoodStock( - enReceive );
            gfRun.IncEnergy   ( enReceive );
        }
        break;

	    case Action::Id::undefined:
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
    GridPoint             const gp, 
    short                 const sIntensity, // percent value
    Strategy::Id       strategy
)
{
	assert( sIntensity >= 0 );
	if ( m_random.NextRandomNumber( ) < static_cast<unsigned short>(sIntensity) * Random::MAX_VAL / 100ul )
	{
		GridField & gf       = getGridField( gp );
		bool const  bIsAlive = gf.IsAlive();

		if ( strategy != Strategy::Id::empty )
		{
			if ( strategy == Strategy::Id::random ) 
				strategy = static_cast<Strategy::Id>(m_random.NextRandomNumber() % Strategy::COUNT );

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
	std::function<bool( GridPoint const)> const & func, 
	GridRect const & rect
) const
{
    for ( GRID_Y y = rect.GetTop();  y <= rect.GetBottom(); ++y )
    for ( GRID_X x = rect.GetLeft(); x <= rect.GetRight();  ++x )
	{
		GridPoint gp{ x, y };

        if ( func( gp ) )
        {
            return gp;
        }
	}
    return GridPoint::NULL_VAL();
}
