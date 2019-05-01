// grid_model.cpp :
//

#include "stdafx.h"
#include "assert.h"
#include <cstdlib> 
#include <cmath> 
#include "EventInterface.h"
#include "ObserverInterface.h"
#include "strategy.h"
#include "individual.h"
#include "interaction.h"
#include "grid_model.h"
#include "gplIterator.h"
#include "gpList.h"
#include "config.h"
#include "dump.h"
#include "genome.h"
#include "gridPOI.h"
#include "gridField.h"
#include "gridRect.h"

//#ifndef NDEBUG
//    #define CHECK_INDIVIDUALS CheckIndividuals( * this )
//#else
    #define CHECK_INDIVIDUALS
//#endif

ObserverInterface * Grid::m_pObservers = nullptr;    // GUI call back for display of current model 
EventInterface    * Grid::m_pEventPOI  = nullptr;

GROWTH_RATE        Grid::m_enFoodGrowthRate;
ENERGY_UNITS       Grid::m_enBasicFoodConsumption;
ENERGY_UNITS       Grid::m_enMemSizeFoodConsumption;
ENERGY_UNITS       Grid::m_enMoveFoodConsumption;
ENERGY_UNITS       Grid::m_enCloneFoodConsumption;
ENERGY_UNITS       Grid::m_enMarryFoodConsumption;
ENERGY_UNITS       Grid::m_enInteractFoodConsumption;

bool               Grid::m_bNeighborhoodFoodSensitivity;

void Grid::RefreshCache( )
{
	m_bNeighborhoodFoodSensitivity = Config::GetConfigValueBool( Config::tId::neighborhoodFoodSensitivity );
	m_enFoodGrowthRate             = GROWTH_RATE (Config::GetConfigValueShort( Config::tId::growthRateFood ));
	m_enBasicFoodConsumption       = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionBasicRate ));
	m_enMemSizeFoodConsumption     = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionMemSize ));
	m_enMoveFoodConsumption        = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionMove ));
	m_enCloneFoodConsumption       = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionClone ));
	m_enMarryFoodConsumption       = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionMarry ));
	m_enInteractFoodConsumption    = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionInteraction ));
	INTERACTION::RefreshCash( );
	Individual::RefreshCache( );
	GridField::RefreshCash( );
	Genome::RefreshCash( );
}

void Grid::InitClass
( 
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
	m_pObservers = pObservers; 
	m_pEventPOI  = pEvent; 
	Genome::InitClass( );
	RefreshCache( );
}

Grid::Grid( )
    : m_gpList( ),
      m_random( ),
      m_idCounter( ),
      m_genEvo( 0L ),
	  m_emptyNeighborSlots( ),
	  m_occupiedNeighborSlots( )
{
	m_aGF.resize( GridDimensions::GridWidthVal() );
	for ( auto & col: m_aGF )
		col.resize( GridDimensions::GridHeightVal() );

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

BYTES Grid::GetGridExtraSize()
{
	unsigned long long gridFieldSize { sizeof ( GridField ) };
	unsigned long long gridRowSize   { sizeof(vector< GridField >) + GridDimensions::GridHeightVal() * gridFieldSize };
	unsigned long long gridAreaSize  { GridDimensions::GridWidthVal() * gridRowSize };
	return BYTES( gridAreaSize );
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
    Apply2Grid( [&](GridPoint const gp) { getGridField( gp ).ResetGridField( enFood ); } );
    m_gpList.ResetGpList( );
    m_random.InitializeRandom( );
}

void Grid::stopOnPoi( GridField const & gfRun )
{
	if ( GridPOI::IsPoi( gfRun.GetId( ) ) )
	{
		m_pObservers->Notify( true );
		m_pEventPOI->Wait( );
	}
}

GridPoint Grid::chooseTarget( Neighborhood & gpListEmpty )
{
    if ( m_bNeighborhoodFoodSensitivity )
        getBestNeighborSlots( gpListEmpty );   // consider only neighbor slots with best food stock

    GridPoint m_gpTarget = gpListEmpty.GetRandomElement( m_random.NextRandomNumber( ) ); // choose one of them randomly
    assert( IsDead( m_gpTarget ) );
    return m_gpTarget;
}

GridPoint Grid::choosePartner( Neighborhood & gpListFilled )
{
    GridPoint const m_gpPartner = gpListFilled.GetRandomElement( m_random.NextRandomNumber( ) );
    assert( IsAlive( m_gpPartner ) );
    return m_gpPartner;
}

GridPoint Grid::actionMove( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.SetTarget( chooseTarget( m_emptyNeighborSlots ) );
	m_plan.NoPartner( );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	gfRun.DecEnergy( m_enMoveFoodConsumption );
	if ( gfRun.IsAlive( ) )
	{
		GridField & gfTarget = getGridField( m_plan.GetTarget( ) );
		m_gpList.ReplaceGridPointInList( * this, gfRun, gfTarget ); 
		gfTarget.MoveIndividual( gfRun );
	}
	else 
	{
		deleteAndReset( gfRun );
	}
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionClone( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.SetTarget( chooseTarget( m_emptyNeighborSlots ) );
	m_plan.NoPartner( );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	GridField & gfTarget = getGridField( m_plan.GetTarget( ) );
	gfRun.DecEnergy( m_enCloneFoodConsumption );
	gfTarget.CloneIndividual( ++m_idCounter, m_genEvo, m_random, gfRun );
	if ( gfTarget.IsAlive( ) )
		m_gpList.AddGridPointToList( *this, gfTarget );
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionMarry( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.SetPartner( choosePartner( m_occupiedNeighborSlots ) );
	m_plan.SetTarget ( chooseTarget ( m_emptyNeighborSlots ) );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	GridField & gfTarget  = getGridField( m_plan.GetTarget( ) );
	GridField & gfPartner = getGridField( m_plan.GetPartner( ) );
	gfRun.DecEnergy( m_enMarryFoodConsumption );
	gfTarget.BreedIndividual( ++m_idCounter, m_genEvo, m_random, gfRun, gfPartner );
	if ( gfTarget.IsAlive( ) )
		m_gpList.AddGridPointToList( *this, gfTarget );
	deleteIfDead( gfPartner );
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionInteract( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.SetPartner( choosePartner( m_occupiedNeighborSlots ) );
	m_plan.NoTarget( );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	GridField & gfPartner = getGridField( m_plan.GetPartner( ) );
	gfRun.DecEnergy( m_enInteractFoodConsumption );
	GridField::Interact( gfRun, gfPartner );
	if ( gpNext.IsNotNull( ) )
	{
		GridField & gfNext = getGridField( gpNext );
		if ( gfNext.IsDead( ) )
			gpNext = gfNext.GetJuniorGp( );
	}
	deleteIfDead( gfPartner );
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionPassOn( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.NoPartner( );
	m_plan.NoTarget ( );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionFertilize( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.NoPartner( );
	m_plan.NoTarget ( );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	ENERGY_UNITS const enInvest = ENERGY_UNITS(gfRun.GetAllele( GeneType::Id::fertilInvest ));
	gfRun.Fertilize( enInvest );
	gfRun.DecEnergy( enInvest );
	gfRun.PassOn2Child( ++m_idCounter, m_genEvo, m_random );
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionEat( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	m_plan.NoPartner( );
	m_plan.NoTarget ( );
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	{
		ENERGY_UNITS const enWant    = ENERGY_UNITS( gfRun.GetAllele( GeneType::Id::appetite ) );
		ENERGY_UNITS const enReceive = gfRun.GetConsumption( enWant );
		gfRun.IncFoodStock( -enReceive );
		gfRun.IncEnergy   (  enReceive );
	}
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::actionUndefined( GridField & gfRun )
{
	GridPoint gpNext = gfRun.GetJuniorGp( );      // may be NULL_VAL
	stopOnPoi( gfRun );
	gfRun.DecEnergy( m_plan.GetBaseConsumption( ) );
	incActionCounter( gfRun.GetStrategyId( ), m_plan.GetActionType( ) );
	deleteIfDead( gfRun );
	assert( (gpNext.IsNull() ) || IsAlive( gpNext ) );
	return gpNext;
}

GridPoint Grid::GenerationStep( GridPoint const gpRun )
{
	GridPoint   gpNext( GP_NULL );
	GridField & gfRun = getGridField( gpRun );
    assert( gfRun.IsAlive( ) );

	m_plan.SetActor( gpRun );

    // basic food consumption

	m_plan.SetBaseConsumption( m_enBasicFoodConsumption );
	m_plan.IncBaseConsumption( m_enMemSizeFoodConsumption * gfRun.GetMemSize( ).GetValue() );

	// inspect neighborhood

	m_emptyNeighborSlots.Clear( );
	m_occupiedNeighborSlots.Clear( );

	for ( auto gp: GridDimensions::GetNeighbors( gpRun ) )
	{
		(IsAlive(gp) ? m_occupiedNeighborSlots : m_emptyNeighborSlots).AddToList( gp );
	}

    assert( m_emptyNeighborSlots.GetLength() + m_occupiedNeighborSlots.GetLength() == Neighborhood::GetNrOfNeighbors( ) );

	// decide on action

    Action::Id action = gfRun.GetGenome( ).GetOption
	( 
        m_emptyNeighborSlots.GetLength( ) > 0,              // has free space around? 
        m_occupiedNeighborSlots.GetLength( ) > 0,           // has neighbor
        gfRun.GetEnergy( ) - m_plan.GetBaseConsumption( ),  // available energy
	    GetAge( gpRun ),
		m_random 
	);

	m_plan.SetActionType( action );

    switch ( action )
    {
		case Action::Id::move:	    return actionMove     ( gfRun );
		case Action::Id::clone:     return actionClone    ( gfRun );
		case Action::Id::marry:     return actionMarry    ( gfRun );
		case Action::Id::interact:  return actionInteract ( gfRun );
		case Action::Id::passOn:    return actionPassOn   ( gfRun );
		case Action::Id::fertilize: return actionFertilize( gfRun );
		case Action::Id::eat:       return actionEat      ( gfRun );
		case Action::Id::undefined:	return actionUndefined( gfRun );
		default: assert( false );
    }

	return GP_NULL;
}

void Grid::EditSetStrategy
( 
    GridPoint const gp, 
    PERCENT   const intensity, // percent value
    Strategy::Id    strategy
)
{
	assert( intensity >= 0_PERCENT );
	if ( m_random.NextRandomNumber( ) < intensity.GetValue() * Random::MAX_VAL / 100ul )
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
    for ( GRID_COORD y = rect.GetTop();  y <= rect.GetBottom(); ++y )
    for ( GRID_COORD x = rect.GetLeft(); x <= rect.GetRight();  ++x )
	{
		GridPoint gp{ x, y };

        if ( func( gp ) )
        {
            return gp;
        }
	}
    return GP_NULL;
}
