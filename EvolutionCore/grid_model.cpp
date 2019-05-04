// grid_model.cpp :
//

#include "stdafx.h"
#include "assert.h"
#include <cstdlib> 
#include <cmath> 
#include "EventInterface.h"
#include "MortalityTable.h"
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

using std::endl;

ObserverInterface * Grid::m_pObservers = nullptr;    // GUI call back for display of current model 
EventInterface    * Grid::m_pEventPOI  = nullptr;
wostream          * Grid::m_pProtocol  = nullptr;

void Grid::RefreshCache( )
{
	m_bPassOnEnabled               = Config::GetConfigValueBool( Config::tId::passOnEnabled );
	m_bNeighborhoodFoodSensitivity = Config::GetConfigValueBool( Config::tId::neighborhoodFoodSensitivity );
	m_enFoodGrowthRate             = GROWTH_RATE (Config::GetConfigValueShort( Config::tId::growthRateFood ));
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
	EventInterface    * const pEvent,
	wostream          * const pProtocol
)
{
	m_pObservers = pObservers; 
	m_pEventPOI  = pEvent; 
	m_pProtocol  = pProtocol;
	Genome::InitClass( );
	MortalityTable::InitClass( );
}

Grid::Grid( )
    : m_gpList( ),
      m_random( ),
      m_idCounter( ),
 	  m_bPOI( false ),
	  m_genEvo( 0L ),
	  m_emptyNeighborSlots( ),
	  m_occupiedNeighborSlots( ),
	  m_pDisplayList( nullptr ),
	  m_gpNext( GP_NULL ),     
	  m_gpTarget( GP_NULL ),     
	  m_gpPartner( GP_NULL ),    
      m_strExplanation( nullptr ),
      m_action( Action::Id::undefined ),
      m_enBaseConsumption( 0_ENERGY_UNITS )
{
	RefreshCache( );

	m_aGF.resize( GridDimensions::GridWidthVal() );
	for ( auto & col: m_aGF )
		col.resize( GridDimensions::GridHeightVal() );

    Apply2Grid    // initialization of grid variables which never change after initialization
	( 
    	[&](GridPoint const gp)
		{
           getGridField( gp ).InitGridFieldStructure( gp );
		   return false;
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

BYTES const Grid::GetGridHeapSize() const
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
			return false;
		}
	);

	int const iNrOfLivingIndividuals = grid.GetNrOfLivingIndividuals( );
    assert( iCount == iNrOfLivingIndividuals );
}

void Grid::ResetGrid( )
{
    ENERGY_UNITS enFood = ENERGY_UNITS( Config::GetConfigValueShort( Config::tId::minFood ) );
    Apply2Grid( [&](GridPoint const gp) { getGridField( gp ).ResetGridField( enFood ); return false; } );
    m_gpList.ResetGpList( );
    m_random.InitializeRandom( );
}

void Grid::displayAndWait( )
{
	m_pObservers->Notify( true );
	m_pEventPOI->Wait( );
}

GridField & Grid::chooseTarget( )
{
	if ( m_bPOI )
	{
		* m_pProtocol << L"free slots: " << m_emptyNeighborSlots.GetLength() << endl;
		m_pDisplayList = & m_emptyNeighborSlots;
		displayAndWait( );
	}
	
	if ( m_bNeighborhoodFoodSensitivity )
	{
		ENERGY_UNITS enBest = getBestNeighborSlots( m_emptyNeighborSlots );   // consider only neighbor slots with best food stock
		if ( m_bPOI )
		{
			* m_pProtocol << L"best free slots: " << m_emptyNeighborSlots.GetLength() 
				          << L" slots with "      << enBest << L" energy units each" << endl;
			m_emptyNeighborSlots.Apply2All( [&](GridPoint const gp)	{ * m_pProtocol << gp << endl; } );
			displayAndWait( );
		}
	}

	m_gpTarget = m_emptyNeighborSlots.GetRandomElement( m_random.NextRandomNumber( ) ); // choose one of them randomly
    assert( IsDead( m_gpTarget ) );
	
	if ( m_bPOI )
	{
		* m_pProtocol << L"selected target: " << m_gpTarget << endl;
		m_pDisplayList = nullptr;
		displayAndWait( );
	}

	return getGridField( m_gpTarget );
}

GridField & Grid::choosePartner( )
{
    m_gpPartner = m_occupiedNeighborSlots.GetRandomElement( m_random.NextRandomNumber( ) );
	assert( IsAlive( m_gpPartner ) );
	if ( m_bPOI )
	{
		* m_pProtocol << L"selected partner: " << m_gpPartner << endl;
		m_pDisplayList = nullptr;
		displayAndWait( );
	}
	return getGridField( m_gpPartner );
}

void Grid::actionMove( GridField & gfRun )
{
	GridField & gfTarget = chooseTarget( );
	gfRun.DecEnergy( m_enMoveFoodConsumption );
	if ( gfRun.IsAlive( ) )
	{
		m_gpList.ReplaceGridPointInList( * this, gfRun, gfTarget ); 
		gfTarget.MoveIndividual( gfRun );
		if ( GridPOI::IsPoi( gfRun.GetGridPoint() ) )
			GridPOI::SetPoi( m_gpTarget );
	}
	else 
	{
		deleteAndReset( gfRun );
	}
}

void Grid::actionClone( GridField & gfRun )
{
	GridField & gfTarget = chooseTarget( );
	gfRun.DecEnergy( m_enCloneFoodConsumption );
	gfTarget.CloneIndividual( ++m_idCounter, m_genEvo, m_random, gfRun );
	if ( gfTarget.IsAlive( ) )
		m_gpList.AddGridPointToList( * this, gfTarget );
	deleteIfDead( gfRun );
}

void Grid::actionMarry( GridField & gfRun )
{
	GridField & gfPartner = choosePartner( );
	GridField & gfTarget  = chooseTarget ( );
	gfRun.DecEnergy( m_enMarryFoodConsumption );
	gfTarget.BreedIndividual( ++m_idCounter, m_genEvo, m_random, gfRun, gfPartner );
	if ( gfTarget.IsAlive( ) )
		m_gpList.AddGridPointToList( * this, gfTarget );
	deleteIfDead( gfPartner );
	deleteIfDead( gfRun );
}

void Grid::actionInteract( GridField & gfRun )
{
	GridField & gfPartner = choosePartner( );
	gfRun.DecEnergy( m_enInteractFoodConsumption );
	GridField::Interact( gfRun, gfPartner );
	if ( m_gpNext.IsNotNull( ) )
	{
		GridField & gfNext = getGridField( m_gpNext );
		if ( gfNext.IsDead( ) )
			m_gpNext = gfNext.GetJuniorGp( );
	}
	deleteIfDead( gfPartner );
	deleteIfDead( gfRun );
}

void Grid::actionPassOn( GridField & gfRun )
{
	gfRun.PassOn2Child( ++m_idCounter, m_genEvo, m_random );
	deleteIfDead( gfRun );
}

void Grid::actionFertilize( GridField & gfRun )
{
	ENERGY_UNITS const enInvest = ENERGY_UNITS(gfRun.GetAllele( GeneType::Id::fertilInvest ));
	gfRun.Fertilize( enInvest );
	gfRun.DecEnergy( enInvest );
	deleteIfDead( gfRun );
}

void Grid::actionEat( GridField & gfRun )
{
	gfRun.Eat( );
	deleteIfDead( gfRun );
}

void Grid::actionUndefined( GridField & gfRun )
{
	deleteIfDead( gfRun );
}

GridPoint Grid::ComputeNextGeneration( GridPoint const gpRun )
{
	GridField    & gfRun  = getGridField( gpRun );
	ENERGY_UNITS   enBase = gfRun.GetBaseConsumption( );
	EVO_GENERATION age    = getAge( gfRun ); 
	assert( gfRun.IsAlive( ) );

	m_bPOI = GridPOI::IsPoi( gpRun );

	if ( m_bPOI )
	{
		* m_pProtocol << L"*** start processing POI: " << gpRun << endl;
		* m_pProtocol << L"base consumption: " << enBase << endl;
		displayAndWait( );
	}

	gfRun.DecEnergy( enBase );

	if ( m_bPOI )
	{
		* m_pProtocol << L"remaining energy: " << gfRun.GetEnergy() << std::endl;
		displayAndWait( );
	}

	if ( m_bPassOnEnabled && ( MortalityTable::GetRate( age ) > m_random.NextRandomNumber() ) )
	{
		m_action = Action::Id::passOn;
		if ( m_bPOI )
		{
			* m_pProtocol << L"individual dies at age: " << age << std::endl;
			displayAndWait( );
		}
	}
	else
	{
		// inspect neighborhood

		m_emptyNeighborSlots.Clear( );
		m_occupiedNeighborSlots.Clear( );

		for ( auto gp: GridDimensions::GetNeighbors( gpRun ) )
		{
			(IsAlive(gp) ? m_occupiedNeighborSlots : m_emptyNeighborSlots).AddToList( gp );
		}

		if ( m_bPOI )
		{
			* m_pProtocol << L"neighborhood: " 
				<< m_occupiedNeighborSlots.GetLength() << L" occupied, "
				<< m_emptyNeighborSlots.GetLength()    << L" free" << std::endl;
			displayAndWait( );
		}

		assert( m_emptyNeighborSlots.GetLength() + m_occupiedNeighborSlots.GetLength() == Neighborhood::GetNrOfNeighbors( ) );

		// decide on action

		m_options.InitOptions
		( 
			gfRun.GetGenome( ), 
			m_emptyNeighborSlots   .GetLength( ) > 0,  // has free space around? bHasFreeSpace, 
			m_occupiedNeighborSlots.GetLength( ) > 0,  // has neighborbHasNeighbor, 
			gfRun.GetEnergy( ) 
		);
		if ( m_bPOI )
		{
			m_options.DisplayValidOptions( m_pProtocol );
			displayAndWait( );
		}
		unsigned int uiSum = m_options.GetSumOfValidOptions( gfRun.GetGenome( ) );
		unsigned int uiVal = m_random.NextRandomNumberScaledTo( uiSum );
		m_action = m_options.SelectAction( gfRun.GetGenome( ), uiVal );
		if ( m_bPOI )
		{
			* m_pProtocol << L"selected action: " << Action::GetName( m_action ) << endl; 
			displayAndWait( );
		}
	}

	m_gpNext = gfRun.GetJuniorGp( );   

    switch ( m_action )
    {
		case Action::Id::move:	    actionMove     ( gfRun ); break;
		case Action::Id::clone:     actionClone    ( gfRun ); break; 
		case Action::Id::marry:     actionMarry    ( gfRun ); break;
		case Action::Id::interact:  actionInteract ( gfRun ); break;
		case Action::Id::passOn:    actionPassOn   ( gfRun ); break;
		case Action::Id::fertilize: actionFertilize( gfRun ); break;
		case Action::Id::eat:       actionEat      ( gfRun ); break;
		case Action::Id::undefined:	actionUndefined( gfRun ); break;
		default: assert( false );
    }

	assert( (m_gpNext.IsNull() ) || IsAlive( m_gpNext ) );

	if ( gfRun.IsAlive( ) )
		incActionCounter( gfRun.GetStrategyId( ), m_action );

	m_bPOI = false;
	m_strExplanation = nullptr;

	return m_gpNext;    // may be NULL_VAL
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

GridPoint const Grid::FindGridPoint
(
	GridPointFunc const & func, 
	GridRect      const & rect
) const
{
	for ( GRID_COORD y = rect.GetTop();  y <= rect.GetBottom(); ++y )
	for ( GRID_COORD x = rect.GetLeft(); x <= rect.GetRight();  ++x )
	{
		GridPoint gp { x, y };
		if ( func( gp ) )
		{
			return gp;
		}
	}
	return GP_NULL;
}

GridPoint const Grid::FindGridPointFromId( IND_ID const idSearchedFor ) const 
{ 
	return ( idSearchedFor.IsNull() )
     		? GP_NULL
			: FindGridPoint
			( 
				[&](GridPoint const gp) 
				{ 
					return ( GetId( gp ) == idSearchedFor );
				}, 
				GridDimensions::GridRectFull() 
			);
}
