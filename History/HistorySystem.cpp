// historyComputation.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "hist_slot.h"
#include "historyCache.h"
#include "historyIterator.h"
#include "HistoryGeneration.h"
#include "DisplayFunctor.h"
#include "NextGenFunctor.h"
#include "HistorySystem.h"
#include "genCmdList.h"

// public member functions

HistorySystem::HistorySystem( ) :
    m_pGenCmdList( nullptr ),
    m_pHistoryCache( nullptr ),
    m_pHistCacheItemWork( nullptr ),
    m_pAskHistoryCutFunctor( nullptr ),
    m_pNextGenerationFunctor( nullptr )
{ }

HistorySystem::~HistorySystem( )
{
    delete m_pGenCmdList;
    delete m_pHistoryCache;

    m_pGenCmdList = nullptr;
    m_pHistoryCache = nullptr;
    m_pHistCacheItemWork = nullptr;
    m_pAskHistoryCutFunctor = nullptr;
    m_pNextGenerationFunctor = nullptr;
}

void HistorySystem::InitHistorySystem
(
    short           const         sNrOfSlots,
    HIST_GENERATION const         genMaxNrOfGens,
    NextGenFunctor  const * const pNextGenerationFunctor,
    ModelFactory          *       pModelFactory
)
{
    m_pNextGenerationFunctor = pNextGenerationFunctor;
	m_pModelFactory          = pModelFactory;
    m_pGenCmdList            = new GenCmdList( genMaxNrOfGens );
    m_pHistoryCache          = new HistoryCache;
	m_pHistCacheItemWork     = HistCacheItem::CreateItem( pModelFactory );

    m_pHistoryCache->InitHistoryCache( sNrOfSlots, pModelFactory );
    m_pHistCacheItemWork->SetGenerationCommand( GenerationCmd::RESET );
    save2History( * m_pHistCacheItemWork );
}

bool HistorySystem::AddHistorySlot( ) const 
{ 
    return m_pHistoryCache->AddCacheSlot( m_pHistCacheItemWork, m_pModelFactory );
}

int HistorySystem::GetNrOfHistCacheSlots( ) const 
{ 
    return m_pHistoryCache->GetNrOfHistCacheSlots( ); 
}

HIST_GENERATION HistorySystem::GetNrOfGenerations( ) const 
{ 
    return m_pHistoryCache->GetYoungestGeneration( ) + 1; 
}

HIST_GENERATION HistorySystem::GetYoungestGeneration( ) const 
{ 
    return m_pHistoryCache->GetYoungestGeneration( ); 
}

HIST_GENERATION HistorySystem::GetCurrentGeneration( ) const 
{ 
    return m_pHistCacheItemWork->GetHistGenCounter( ); 
}

bool HistorySystem::IsInHistoryMode( ) const 
{ 
    return GetCurrentGeneration( ) < m_pHistoryCache->GetYoungestGeneration( ); 
};

void HistorySystem::ShutDownHistCacheSlot( short const i ) 
{ 
    m_pHistoryCache->ShutDownHistCacheSlot( i ); 
}

HistoryIterator * HistorySystem::CreateHistoryIterator( ) const 
{ 
    return new HistoryIterator( m_pHistoryCache ); 
}

// ClearHistory
// Erase all generations starting with genFirst up to youngest generation
// from m_GenerationCmd and from history slots

void HistorySystem::ClearHistory( HIST_GENERATION const genFirst )
{
	for ( HIST_GENERATION gen = GetYoungestGeneration(); gen > genFirst; --gen )
	{
		if  (m_pGenCmdList->IsCachedGeneration( gen ) )
			m_pHistoryCache->RemoveHistCacheSlot( ( * m_pGenCmdList)[gen].GetParam( ) );

		m_pGenCmdList->ResetGenerationCmd(gen);
	}
}

void HistorySystem::createNewGen( GenerationCmd genCmd )
{
	CHECK_HISTORY_STRUCTURE;
    step2NextGeneration( genCmd );
    m_pHistCacheItemWork->SetGenerationCommand( genCmd );
    save2History( * m_pHistCacheItemWork );
    CHECK_HISTORY_STRUCTURE;
}

bool HistorySystem::CreateNewGeneration( unsigned short const uiCmd, short const sParam )
{
	if ( GetYoungestGeneration() != GetCurrentGeneration() ) // If in history mode: erase all future generations
	{
		assert( m_pAskHistoryCutFunctor != nullptr );
		if ( ! (  * m_pAskHistoryCutFunctor ) (false  ))  // ask user if really cut off history
			return false;

		ClearHistory( m_pHistCacheItemWork->GetHistGenCounter( ) );
	}

	createNewGen( GenerationCmd( uiCmd, sParam ) );

	return true;
}

// ApproachHistGen - Get closer to demanded HIST_GENERATION
//                 - If several steps are neccessary, function returns after one displayed generation
//                   to allow user interaction
//                 - But actual history generation as alterered by at least 1

void HistorySystem::ApproachHistGen( HIST_GENERATION const genDemanded )
{
    HIST_GENERATION genActual = m_pHistCacheItemWork->GetHistGenCounter( );
    BOOL            bMicrosteps = TRUE;

    assert( genDemanded != genActual );
    assert( genDemanded < m_pGenCmdList->GetCmdListSize( ) ); //TODO: find clean solution if max number of generations reached. 

    if ( ( * m_pGenCmdList )[ genDemanded ].IsUndefined( ) )   // normal forward mode
    {
        assert( genDemanded == genActual + 1 );
        createNewGen( GenerationCmd::NEXT_GEN );                           
    }
    else   // genDemanded is somewhere in history
    {
        HIST_GENERATION genCached = genDemanded;  // search backwards starting with genDemanded
        
        while ( ( * m_pGenCmdList )[ genCached ].IsNotCachedGeneration( ) )
            --genCached;

        // now we have found a cached generation  

        if ( 
              (( genCached <= genActual ) && ( genActual < genDemanded )) || // cached generation is not better than actual generation
              (( genActual == genDemanded - 1 ) && bMicrosteps )
           )
        {
            GenerationCmd genCmd = ( * m_pGenCmdList )[ genActual + 1 ];
            step2NextGeneration( genCmd );                              // compute forwards
        }
        else  // get cached generation
        {
            GenerationCmd      const         genCmd         = ( * m_pGenCmdList )[ genCached ];
            short              const         sSlotNr        = genCmd.GetParam( );
            HistCacheItem const * const pHistCacheItem = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
            m_pHistCacheItemWork->CopyCacheItem( pHistCacheItem );
        }
    }
}

// private member functions

// save2History - Save Generation data (rule how to get to next generation), current Grid, etc. to new slot

void HistorySystem::save2History( HistCacheItem const & histCacheItem )
{
    short         const   sSlotNr         = m_pHistoryCache->GetFreeCacheSlotNr( );
    HistCacheItem const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
    GenerationCmd const   genCmdFromCache = pHistCacheItem->GetGenCmd( );

    if ( genCmdFromCache.IsDefined( ) )       // Hist slot was in use before. Save GenCommand
    {
        HIST_GENERATION const genCached = pHistCacheItem->GetHistGenCounter( );
        #ifdef _DEBUG
            GenerationCmd genCmdInList = ( * m_pGenCmdList )[ genCached ];
            assert( genCmdInList.IsCachedGeneration( ) );
            assert( genCmdInList.GetParam( ) == sSlotNr );
        #endif
        m_pGenCmdList->SetGenerationCmd( genCached, genCmdFromCache );
        m_pHistoryCache->ResetHistCacheSlot( sSlotNr );
    }

    m_pHistoryCache->Save2CacheSlot( histCacheItem, sSlotNr );
    m_pGenCmdList->SetCachedGeneration( histCacheItem.GetHistGenCounter( ), sSlotNr );
};

// step2NextGeneration - if cached generation: get GenerationCmd from cache
//                     - apply command and increment history generation counter

void HistorySystem::step2NextGeneration( GenerationCmd genCmd )
{
    if ( genCmd.IsCachedGeneration( ) ) // can happen only in history mode
    {
        assert( IsInHistoryMode( ) );
        short              const   sSlotNr = genCmd.GetParam( );
        HistCacheItem const * pHistCacheItem = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
        genCmd = pHistCacheItem->GetGenCmd( );
    }

	switch ( genCmd.GetCommand() )
	{
	case tGenCmd::nextGen:
		m_pNextGenerationFunctor->OnNextGeneration( );
		break;

	case tGenCmd::reset:
	    m_pNextGenerationFunctor->OnReset( );
		break;

	default:
        m_pNextGenerationFunctor->OnAppCommand   // Apply application defined operation to step to next generation
		( 
			genCmd.GetAppCmd( ),
			genCmd.GetParam( ) 
		); 
	}

    m_pHistCacheItemWork->IncHistGenCounter( );

    assert( m_pHistCacheItemWork->GetHistGenCounter( ) < m_pGenCmdList->GetCmdListSize( ) ); //TODO: find clean solution if max number of generations reached. 
}

HIST_GENERATION HistorySystem::FindFirstGenerationWithProperty( GenerationProperty const & property ) const
{
    HistoryIterator histIter( m_pHistoryCache );    // we move forwards thru time, starting with oldest generation.

    for ( HistSlot const * phsRun = histIter.Set2Oldest( ); phsRun != nullptr; phsRun = histIter.Set2Junior( ) )
    {
        if ( (property)( phsRun->GetHistCacheItemC( ) ) )
        {
            return phsRun->GetGridGeneration( );
        }
    }

    return -1;
}

HIST_GENERATION HistorySystem::FindLastGenerationWithProperty( GenerationProperty const & property ) const
{
    HistoryIterator histIter( m_pHistoryCache );    // we move backwards thru time, starting with youngest generation.

    for ( HistSlot const * phsRun = histIter.Set2Youngest( ); phsRun != nullptr; phsRun = histIter.Set2Senior( ) )
    {
        if ( (property)( phsRun->GetHistCacheItemC( ) ) ) 
        {
            return phsRun->GetGridGeneration( );
        }
    }

    return -1;
}

void HistorySystem::checkHistoryStructure( )  // used only in debug mode
{
     // check generation number of all generations in history cache

    for ( HIST_GENERATION gen = 0; gen < m_pGenCmdList->GetCmdListSize( ); ++gen )
    {
        GenerationCmd generationCmd = ( * m_pGenCmdList )[ gen ];
        if ( generationCmd.IsCachedGeneration( ) )
        {
            short              const   sSlotNrFromList = generationCmd.GetParam( );
            HistCacheItem const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC( sSlotNrFromList );
            HIST_GENERATION    const   genNrFromCache  = pHistCacheItem->GetHistGenCounter( );
            assert( genNrFromCache == gen );
        }
        else if ( generationCmd.IsUndefined( ) )
            break;
    }

    // check slot numbers and according generation commands

    for ( short sSlotNr = 0; sSlotNr < m_pHistoryCache->GetNrOfHistCacheSlots( ); ++sSlotNr )
    {
        HistCacheItem const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
        HIST_GENERATION    const   genNrFromCache  = pHistCacheItem->GetHistGenCounter( );
        GenerationCmd      const   genCmdFromCache = pHistCacheItem->GetGenCmd( );
        if ( genCmdFromCache.IsDefined( ) )
        {
            GenerationCmd const generationCmd   = ( * m_pGenCmdList )[ genNrFromCache ];
            short         const sSlotNrFromList = generationCmd.GetParam( );
            assert( generationCmd.IsCachedGeneration( ) );
            assert( sSlotNrFromList == sSlotNr );
        }
    }
}
