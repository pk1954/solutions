// grid_model.cpp :
//

#include "stdafx.h"
#include <array> 
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
#include "gridCircle.h"
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
      m_uiFoodGrowth( 0 ),
      m_random( ),
      m_idCounter( ),
      m_genEvo( 0L )
{
    class initGridFields : public GridPoint_Functor
    {
    public:
        explicit initGridFields( Grid * const pGrid ) : GridPoint_Functor(pGrid) { };        
 
        virtual void operator() ( GridPoint const & gpRun )
        {
            GetGrid( )->getGridField( gpRun ).InitGridFieldStructure( gpRun );
        }
    };

    Apply2Grid( & initGridFields( this ) );    // initialization of grid variables which never change after initialization
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

//lint -e1763         operator() marked as const indirectly modifies class
void CheckIndividuals( Grid & grid )
{
    class check : public GridPoint_Functor
    {
    public:
        check( Grid * const pGrid, int * const piCount ) 
        : GridPoint_Functor( pGrid ), 
          m_piCount( piCount ) 
        { };

        virtual void operator() ( GridPoint const & gp )
        {
            if ( GetGrid()->IsAlive( gp ) )
               ++ (* m_piCount);
        }

    private:
        int * const m_piCount;
    };

    int iCount = 0;
    check functor( & grid, & iCount );
    Apply2Grid( & functor );
    int const iNrOfLivingIndividuals = grid.GetNrOfLivingIndividuals( );
    assert( iCount == iNrOfLivingIndividuals );
}
//lint +e1763 

void Grid::ResetGrid( )
{
    class reset : public GridPoint_Functor
    {
    public:
        reset( Grid * const pGrid, short const sFood ) 
        : GridPoint_Functor( pGrid ), 
          m_sFood( sFood ) 
        { }
 
        virtual void operator() ( GridPoint const & gpRun )
        {
            GetGrid( )->getGridField( gpRun ).ResetGridField( m_sFood );
        }

    private:
        short m_sFood;
    };

    Apply2Grid( & reset( this, Config::GetConfigValueShort( Config::tId::minFood ) ) );
    m_gpList.ResetGpList( );
    m_random.InitializeRandom( );
}

// getSlots: visit direct neighbors of a given GridPoint and create a list of all neighbors
//           satisfying a criterion given as a Decision_Functor

class Grid::getSlots : public GridPointNeighbor_Functor
{
public:
    getSlots
    ( 
        Grid             const & grid,
        Decision_Functor const & func, 
        GridPoint        const & gp, 
        NeighborList           & list
    ) : 
        m_grid( grid ),
        m_func( func ),
        GridPointNeighbor_Functor( gp ),
        m_list( list )
    { }

    virtual ~getSlots() { };
 
    virtual bool operator() ( GridPoint const & gpNeighbor ) const 
    {
        if ( (m_func)( m_grid.GetGridField( gpNeighbor ) ) )
        {
            m_list.AddToList( gpNeighbor );
        }
        return false;
    }
private:
//lint -e1725                          // reference members
    Grid             const & m_grid;
    Decision_Functor const & m_func; 
    NeighborList           & m_list;
//lint +e1725
};

GridPoint Grid::chooseTarget( NeighborList & gpListEmpty )
{
    GridPoint m_gpTarget;

    if ( m_bNeighborhoodFoodSensitivity )
        gpListEmpty = getBestNeighborSlots( gpListEmpty );      // find free neighbor slots with best food stock

    unsigned int const uiRandom = m_random.NextRandomNumber( );
    m_gpTarget = gpListEmpty.GetRandomElement( uiRandom ); // choose one of them at random
    assert( IsDead( m_gpTarget ) );

    return m_gpTarget;
}

GridPoint Grid::choosePartner( NeighborList & gpListFilled )
{
    GridPoint m_gpPartner;

    unsigned int const uiRandom = m_random.NextRandomNumber( );
    m_gpPartner = gpListFilled.GetRandomElement( uiRandom );
    assert( IsAlive( m_gpPartner ) );

    return m_gpPartner;
}

void Grid::MakePlan
( 
    GridPoint       const & gpRun, 
    PlannedActivity       & plan
)
{
    GridField const & gfRun = GetGridField( gpRun );
    assert( gfRun.IsAlive( ) );

    plan.SetActor( gpRun );

    // basic food consumption

    plan.SetBaseConsumption( m_iBasicFoodConsumption );
    plan.IncBaseConsumption( m_iMemSizeFoodConsumption * gfRun.GetMemSize( ) );

    NeighborList gpListEmpty;
    NeighborList gpListFilled;

    Apply2AllNeighbors( getSlots( * this, IsDead_Functor(),  gpRun, gpListEmpty  ) );
    Apply2AllNeighbors( getSlots( * this, IsAlive_Functor(), gpRun, gpListFilled ) );

    assert( gpListEmpty.GetLength() +  gpListFilled.GetLength() == NeighborhoodIterator::NR_OF_NEIGHBOURS);

    bool const bHasFreeSpace = gpListEmpty.GetLength( ) > 0;
    bool const bHasNeighbor  = gpListFilled.GetLength( ) > 0;
    int  const iEnergy       = gfRun.GetEnergy( ) - plan.GetBaseConsumption( );

    Genome const & genome = gfRun.GetGenome( );
    tAction        action = genome.GetOption( bHasFreeSpace, bHasNeighbor, iEnergy, m_random );

    plan.SetActionType( action );

    switch ( action )
    {
    case tAction::move:	// choose a free neighbour cell and move there
    case tAction::clone:  // choose a free neighbour cell and create a clone there 
        plan.SetTarget ( chooseTarget( gpListEmpty ) );
        plan.NoPartner( );
        break;

    case tAction::marry:     // choose a living neighbour and marry
        plan.SetPartner( choosePartner( gpListFilled ) );
        plan.SetTarget ( chooseTarget ( gpListEmpty ) );
        break;

    case tAction::interact:  // choose a living neighbour and interact
        plan.SetPartner( choosePartner( gpListFilled ) );
        plan.NoTarget( );
        break;

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
    GridPoint       const & gpRun, 
    PlannedActivity const & plan
)  
{
    GridField & gfRun  = getGridField( gpRun );
    GridPoint   gpNext = gfRun.GetJuniorGp( );      // may be GP_NULL

    gfRun.SetLastAction( plan.GetActionType( ) );
    gfRun.DecEnergy    ( plan.GetBaseConsumption( ) );

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
            }
            else // caution: Do not remove this line!! gfRun is not the same as before  
            {
                deleteAndReset( gfRun );
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

            deleteIfDead( gfRun );
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
            deleteIfDead( gfRun );
        }
        break;

        case tAction::interact:
        {
            GridField & gfTarget = getGridField( plan.GetPartner( ) );
            gfRun.DecEnergy( m_iInteractFoodConsumption );
            GridField::Interact( gfRun, getGridField( plan.GetPartner( ) ) );

            if ( gpNext.IsNotNull( ) )
            {
                GridField & gfNext = getGridField( gpNext );
                if ( gfNext.IsDead( ) )
                    gpNext = gfNext.GetJuniorGp( );
            }

            deleteIfDead( gfTarget );
            deleteIfDead( gfRun );
        }
        break;

        case tAction::fertilize:
        {
            short const sInvest = gfRun.GetAllele( tGeneType::fertilInvest );
            gfRun.Fertilize( sInvest );
            gfRun.DecEnergy( sInvest );
            deleteIfDead( gfRun );
        }
        break;

        case tAction::eat:  
        {
            short const sWant    = gfRun.GetAllele( tGeneType::appetite );
            short const sReceive = gfRun.GetConsumption( sWant );
            gfRun.DecFoodStock( sReceive );
            gfRun.IncEnergy   ( sReceive );
            deleteIfDead( gfRun );
        }
        break;

	    case tAction::undefined:
        break;

        default:
            assert( false );
    }

    CHECK_INDIVIDUALS;
    return gpNext;
}

void Grid::EditSetStrategy
( 
    GridPoint     const & gp, 
    unsigned int  const uiIntensity, // percent value
    tStrategyId   const s
)
{
    unsigned int const uiRand  = NextRandomNumber( );
    unsigned int const uiLimit = uiIntensity * RAND_MAX / 100L;

    if ( uiRand < uiLimit )
    {
        GridField & gf       = getGridField( gp );
        bool const  bIsAlive = gf.IsAlive();

        if ( s != tStrategyId::empty )
        {
            tStrategyId const strategy = ( s == tStrategyId::random ) 
                                       ? static_cast<tStrategyId>(m_random.NextRandomNumber() % NR_STRATEGIES ) 
                                       : s;
            gf.CreateIndividual( ++m_idCounter, m_genEvo, strategy );
            if ( ! bIsAlive )
                m_gpList.AddGridPointToList( * this, gf );
        }
        else if ( bIsAlive )
        {
            deleteAndReset( gf );
        }
    };
}

GridPoint Grid::FindGridPoint( IndId const & id ) const  //TODO: Optimize with hint on previous position
{
    GridPoint gp;
    for ( gp.y = GridRect::GRID_RECT_FULL.GetTop ( ); gp.y <= GridRect::GRID_RECT_FULL.GetBottom( ); ++gp.y )
    for ( gp.x = GridRect::GRID_RECT_FULL.GetLeft( ); gp.x <= GridRect::GRID_RECT_FULL.GetRight ( ); ++gp.x )
        if ( GetId( gp ) == id )
        {
            return gp;
        }

    return GridPoint::GP_NULL;
}
