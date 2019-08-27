// grid_model.cpp :
//

#include "stdafx.h"
#include "assert.h"
#include <cstdlib> 
#include <cmath> 
#include "EventInterface.h"
#include "MortalityTable.h"
#include "ViewCollection.h"
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
wostringstream    * Grid::m_pProtocol  = nullptr;

void Grid::RefreshCache( )
{
	m_lFertilizerYield             = Config::GetConfigValue( Config::tId::fertilizerYield );
	m_bPassOnEnabled               = Config::GetConfigValueBool( Config::tId::passOnEnabled );
	m_bNeighborhoodFoodSensitivity = Config::GetConfigValueBool( Config::tId::neighborhoodFoodSensitivity );
	m_enFoodGrowthRate             = GROWTH_RATE (Config::GetConfigValueShort( Config::tId::growthRateFood ));
	m_enMoveFoodConsumption        = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionMove ));
	m_enCloneFoodConsumption       = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionClone ));
	m_enMarryFoodConsumption       = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionMarry ));
	m_enInteractFoodConsumption    = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionInteraction ));
	m_enMaxFertilizer              = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::maxFertilizer ));
	m_enFoodReserve                = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::reserveFood ));
	m_enBasicFoodConsumption       = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionBasicRate ));
	m_enMemSizeFoodConsumption     = ENERGY_UNITS(Config::GetConfigValueShort( Config::tId::energyConsumptionMemSize ));
	INTERACTION::RefreshCash( );
	Individual::RefreshCache( );
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
	m_pProtocol  = new wostringstream();
	Genome::InitClass( );
	MortalityTable::InitClass( );
}

Grid::Grid( )
    : m_gpList( ),
      m_random( ),
      m_idCounter( ),
 	  m_bPOI( false ),
	  m_bPassOnEnabled( true ),
	  m_bNeighborhoodFoodSensitivity( true ),
	  m_genEvo( 0L ),
	  m_emptyNeighborSlots( ),
	  m_occupiedNeighborSlots( ),
	  m_pDisplayList( nullptr ),
	  m_gpRun( GP_NULL ),     
	  m_gpNext( GP_NULL ),     
	  m_gpTarget( GP_NULL ),     
	  m_gpPartner( GP_NULL ),    
      m_action( Action::Id::undefined ),
      m_enBaseConsumption( 0_ENERGY_UNITS ),
      m_enMaxFertilizer( 0_ENERGY_UNITS ),
      m_lFertilizerYield( 0 )

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
    m_random.Initialize( );
}

void Grid::displayAndWait( )
{
	if ( m_pObservers )
		m_pObservers->Notify( true );
	if ( m_pEventPOI )
		m_pEventPOI->Wait( );                // wait for user input to continue
	m_bPOI = GridPOI::IsPoi( m_gpRun );      // POI may have changed in the meantime
}

void Grid::handleBaseConsumption( GridField & gfRun )
{
	ENERGY_UNITS enBefore   = gfRun.GetEnergy();
	ENERGY_UNITS enBaseCons = m_enBasicFoodConsumption + m_enMemSizeFoodConsumption * gfRun.GetMemSize( ).GetValue();
	ENERGY_UNITS enAfter;

	gfRun.DecEnergy( enBaseCons );

	enAfter = gfRun.GetEnergy();

	if ( m_bPOI )
	{
		* m_pProtocol << L"   base consumption: " 
			          << enBefore  .GetValue() << L" - " 
			          << enBaseCons.GetValue() << L" = " 
			          << enAfter   .GetValue() << endl;
		displayAndWait( );
	}
}

void Grid::inspectNeighborHood( )
{
	m_emptyNeighborSlots.Clear( );
	m_occupiedNeighborSlots.Clear( );

	for ( auto gp: GridDimensions::GetNeighbors( m_gpRun ) )
	{
		(IsAlive(gp) ? m_occupiedNeighborSlots : m_emptyNeighborSlots).AddToList( gp );
	}

	if ( m_bPOI )
	{
		* m_pProtocol << L"   neighborhood: " 
			<< m_occupiedNeighborSlots.GetLength() << L" occupied, "
			<< m_emptyNeighborSlots.   GetLength() << L" free" 
			<< endl;
		displayAndWait( );
	}

	assert( m_emptyNeighborSlots.GetLength() + m_occupiedNeighborSlots.GetLength() == Neighborhood::GetNrOfNeighbors( ) );
}

Action::Id Grid::decideOnAction( GridField const & gfRun )
{
	m_options.InitOptions
	( 
		gfRun.GetGenome( ), 
		m_emptyNeighborSlots   .GetLength( ) > 0,  // has free space around? bHasFreeSpace, 
		m_occupiedNeighborSlots.GetLength( ) > 0,  // has neighborbHasNeighbor, 
		gfRun.GetEnergy( ) 
	);
	
	unsigned int const uiSum  = m_options.GetSumOfValidOptions( gfRun.GetGenome( ) );
	unsigned int const uiVal  = m_random.NextRandomNumberScaledTo( uiSum );
	Action::Id   const action = m_options.SelectAction( gfRun.GetGenome( ), uiVal );
		
	if ( m_bPOI )
	{
		m_options.DisplayValidOptions( m_pProtocol, gfRun.GetGenome( ), uiSum );
		* m_pProtocol << L"   selected action: " << Action::GetName( action ) << endl; 
		displayAndWait( );
	}

	return action;
}

GridField & Grid::chooseTarget( )
{
	if ( m_bPOI )
	{
		* m_pProtocol << L"   free slots: " << m_emptyNeighborSlots.GetLength() << endl;
		m_pDisplayList = & m_emptyNeighborSlots;
		displayAndWait( );
	}
	
	if ( m_bNeighborhoodFoodSensitivity )
	{
		ENERGY_UNITS enBest = getBestNeighborSlots( m_emptyNeighborSlots );   // consider only neighbor slots with best food stock
		if ( m_bPOI )
		{
			* m_pProtocol << L"   best free slots: " << m_emptyNeighborSlots.GetLength() 
				          << L" slots with "      << enBest.GetValue() << L" energy units each" << endl;
			m_emptyNeighborSlots.Apply2All( [&](GridPoint const gp)	{ * m_pProtocol << L"   " << gp << endl; } );
			displayAndWait( );
		}
	}

	m_gpTarget = m_emptyNeighborSlots.GetRandomElement( m_random.NextRandomNumber( ) ); // choose one of them randomly
    assert( IsDead( m_gpTarget ) );
	
	if ( m_bPOI )
	{
		* m_pProtocol << L"   selected target: " << m_gpTarget << endl;
		m_pDisplayList = nullptr;
		displayAndWait( );
	}

	return getGridField( m_gpTarget );
}

GridField & Grid::choosePartner( )
{
	if ( m_bPOI )
	{
		* m_pProtocol << L"   partner candidates: " << m_occupiedNeighborSlots.GetLength() << endl;
		m_pDisplayList = & m_occupiedNeighborSlots;
		displayAndWait( );
	}

	m_gpPartner = m_occupiedNeighborSlots.GetRandomElement( m_random.NextRandomNumber( ) );
	assert( IsAlive( m_gpPartner ) );

	if ( m_bPOI )
	{
		printGridPoint( L"   selected partner: ", m_gpPartner );
		m_pDisplayList = nullptr;
		displayAndWait( );
	}

	return getGridField( m_gpPartner );
}

void Grid::actionMove( GridField & gfRun )
{
	GridField & gfTarget = chooseTarget( );
	decEnergy( gfRun, m_enMoveFoodConsumption );
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
	decEnergy( gfRun, m_enCloneFoodConsumption );
	gfTarget.CloneIndividual( ++m_idCounter, m_genEvo, m_random, gfRun );
	long lDonationRate = static_cast<long>( gfRun.GetAllele( GeneType::Id::cloneDonation ) );
	long lParentEnergy = static_cast<long>( gfRun.GetEnergy( ).GetValue() );
	long lDonation = ( lDonationRate * lParentEnergy ) / SHRT_MAX;
	ENERGY_UNITS enDonation = ENERGY_UNITS(CastToShort( lDonation ));
	if ( m_bPOI )
	{
		* m_pProtocol << L"   donation rate : " << lDonationRate << endl;
		* m_pProtocol << L"   current energy: " << lParentEnergy << endl;
	}
	Donate( gfRun, gfTarget, enDonation );
	if ( gfTarget.IsAlive( ) )
		m_gpList.AddGridPointToList( * this, gfTarget );
	deleteIfDead( gfRun );
}

void Grid::actionMarry( GridField & gfRun )
{
	GridField & gfPartner = choosePartner( );
	GridField & gfTarget  = chooseTarget ( );
	decEnergy( gfRun, m_enMarryFoodConsumption );
	gfTarget.BreedIndividual( ++m_idCounter, m_genEvo, m_random, gfRun, gfPartner );
	Donate(     gfRun, gfTarget,     gfRun.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
	Donate( gfPartner, gfTarget, gfPartner.GetEnergy( ) / 3 );   //TODO:  Make variable, Gene?
	if ( gfTarget.IsAlive( ) )
		m_gpList.AddGridPointToList( * this, gfTarget );
	deleteIfDead( gfPartner );
	deleteIfDead( gfRun );
}

void Grid::actionInteract( GridField & gfRun )
{
	GridField & gfPartner = choosePartner( );
	decEnergy( gfRun, m_enInteractFoodConsumption );
	GridField::Interact( gfRun, gfPartner, m_bPOI ? m_pProtocol : nullptr );
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
	if ( m_bPOI )
	{
		* m_pProtocol << L"   individual dies naturally at age: " << getAge( gfRun ).GetValue() << endl;
		displayAndWait( );
	}
	gfRun.PassOn2Child( ++m_idCounter, m_genEvo, m_random );
	deleteIfDead( gfRun );
}

void Grid::actionFertilize( GridField & gfRun )
{
	ENERGY_UNITS const enInvest = ENERGY_UNITS(gfRun.GetAllele( GeneType::Id::fertilInvest ));
	ENERGY_UNITS const yield    = (enInvest * m_lFertilizerYield ) / 100;
	ENERGY_UNITS const newValue = std::min( gfRun.GetFertilizer( ) + yield, m_enMaxFertilizer ); 
	gfRun.SetFertilizer( newValue );
	gfRun.DecEnergy( enInvest );
	deleteIfDead( gfRun );
}

void Grid::actionEat( GridField & gfRun )
{
	ENERGY_UNITS const enWant      = gfRun.GetAppetite( );
	ENERGY_UNITS const enStock     = gfRun.GetFoodStock( );
	ENERGY_UNITS const enAvailable = enStock - m_enFoodReserve;
	ENERGY_UNITS const enReceive   = ENERGY_UNITS( ClipToMinMax( enAvailable, 0_ENERGY_UNITS, enWant ) ); 
	if ( m_bPOI )
	{
		* m_pProtocol << L"   individual wants: " << setw(4) << enWant.GetValue()          << endl;
		* m_pProtocol << L"   food stock:       " << setw(4) << enStock.GetValue()         << endl;
		* m_pProtocol << L"   reserve:          " << setw(4) << m_enFoodReserve.GetValue() << endl;
		* m_pProtocol << L"   available:        " << setw(4) << enAvailable.GetValue()     << endl;
		* m_pProtocol << L"   individual gets:  " << setw(4) << enReceive.GetValue()       << endl;
		displayAndWait( );
	}
	gfRun.IncFoodStock( -enReceive );
	gfRun.IncEnergy   (  enReceive );
	deleteIfDead( gfRun );
}

void Grid::actionUndefined( GridField & gfRun )
{
	deleteIfDead( gfRun );
}

GridPoint Grid::ComputeNextGeneration( GridPoint const gpRun )
{
	m_gpRun = gpRun;
	GridField & gfRun = getGridField( m_gpRun );
	assert( gfRun.IsAlive( ) );

	m_bPOI = GridPOI::IsPoi( m_gpRun );

	if ( m_bPOI )
	{
		* m_pProtocol << endl;
		printGridPoint( L"*** start processing POI: ", m_gpRun );
	}

	handleBaseConsumption( gfRun );

	if ( m_bPassOnEnabled && MortalityTable::IsTimeToDie( getAge( gfRun ), m_random ) )
	{
		m_action = Action::Id::passOn;
	}
	else
	{
		inspectNeighborHood( );
		m_action = decideOnAction( gfRun );
	}

	m_gpNext = gfRun.GetJuniorGp( );   

    switch ( m_action )
    {
		case Action::Id::eat:       actionEat      ( gfRun ); break;
		case Action::Id::move:	    actionMove     ( gfRun ); break;
		case Action::Id::clone:     actionClone    ( gfRun ); break; 
		case Action::Id::marry:     actionMarry    ( gfRun ); break;
		case Action::Id::interact:  actionInteract ( gfRun ); break;
		case Action::Id::passOn:    actionPassOn   ( gfRun ); break;
		case Action::Id::fertilize: actionFertilize( gfRun ); break;
		case Action::Id::undefined:	actionUndefined( gfRun ); break;
		default: assert( false );
    }

	assert( (m_gpNext.IsNull() ) || IsAlive( m_gpNext ) );

	if ( gfRun.IsAlive( ) )
		incActionCounter( gfRun.GetStrategyId( ), m_action );

	m_bPOI = false;
	m_gpTarget.Set2Null();
	m_gpPartner.Set2Null();

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
	GridPointBoolFunc const & func, 
	GridRect          const & rect
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
