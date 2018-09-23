// HistorySystemImpl.cpp
//

#include "stdafx.h"
#include "assert.h"
#include "hist_slot.h"
#include "historyCache.h"
#include "historyIterator.h"
#include "HistoryGeneration.h"
#include "DisplayFunctor.h"
#include "HistorySystemImpl.h"

// public member functions

HistorySystemImpl::HistorySystemImpl( ) :
    m_GenCmdList        ( ),
    m_pHistoryCache     ( nullptr ),
    m_pHistCacheItemWork( nullptr )
{ }

HistorySystemImpl::~HistorySystemImpl( )
{
    delete m_pHistoryCache;

    m_pHistoryCache      = nullptr;
    m_pHistCacheItemWork = nullptr;
}

void HistorySystemImpl::InitHistorySystem
(
    short           const sNrOfSlots,
    HIST_GENERATION const genMaxNrOfGens,
    ModelData     * const pModelDataWork,
    ModelFactory  * const pModelFactory,
	GenerationCmd   const cmd
)
{
    m_pHistoryCache      = new HistoryCache;
	m_pHistCacheItemWork = new HistCacheItem( pModelDataWork );
	m_GenCmdList.Resize( genMaxNrOfGens );
    m_pHistoryCache->InitHistoryCache( sNrOfSlots, pModelFactory );
	m_pHistCacheItemWork->SetGenerationCommand( cmd );
    save2History( );
}

bool HistorySystemImpl::AddHistorySlot( ) const 
{ 
    return m_pHistoryCache->AddCacheSlot( );
}

int HistorySystemImpl::GetNrOfHistCacheSlots( ) const 
{ 
    return m_pHistoryCache->GetNrOfHistCacheSlots( ); 
}

HIST_GENERATION HistorySystemImpl::GetNrOfGenerations( ) const 
{ 
    return m_pHistoryCache->GetYoungestGeneration( ) + 1; 
}

HIST_GENERATION HistorySystemImpl::GetYoungestGeneration( ) const 
{ 
    return m_pHistoryCache->GetYoungestGeneration( ); 
}

HIST_GENERATION HistorySystemImpl::GetCurrentGeneration( ) const 
{ 
    return m_pHistCacheItemWork->GetHistGenCounter( ); 
}

bool HistorySystemImpl::IsInHistoryMode( ) const 
{ 
    return GetCurrentGeneration( ) < m_pHistoryCache->GetYoungestGeneration( ); 
};

void HistorySystemImpl::ShutDownHistCacheSlot( short const i ) 
{ 
    m_pHistoryCache->ShutDownHistCacheSlot( i ); 
}

HistoryIterator * HistorySystemImpl::CreateHistoryIterator( ) const 
{ 
    return new HistoryIterator( m_pHistoryCache ); 
}

// ClearHistory
// Erase all generations starting with genFirst up to youngest generation
// from m_GenerationCmd and from history slots

void HistorySystemImpl::ClearHistory( HIST_GENERATION const genFirst )
{
	for ( HIST_GENERATION gen = GetYoungestGeneration(); gen > genFirst; --gen )
	{
		if  ( m_GenCmdList.IsCachedGeneration( gen ) )
			m_pHistoryCache->RemoveHistCacheSlot( m_GenCmdList[gen].GetSlotNr( ) );

		m_GenCmdList.ResetGenerationCmd( gen );
	}
}

void HistorySystemImpl::CreateAppCommand( GenerationCmd const genCmd )
{
    step2NextGeneration( genCmd );
    m_pHistCacheItemWork->SetGenerationCommand( genCmd );
    save2History( );
}

HistCacheItem const * HistorySystemImpl::getCachedItem( GenerationCmd cmd )
{
	short const sSlotNr = cmd.GetSlotNr( );
    return m_pHistoryCache->GetHistCacheItemC( sSlotNr );
}

// ApproachHistGen - Get closer to demanded HIST_GENERATION
//                 - If several steps are neccessary, function returns after one displayed generation
//                   to allow user interaction
//                 - But actual history generation as alterered by at least 1

void HistorySystemImpl::ApproachHistGen( HIST_GENERATION const genDemanded )
{
    HIST_GENERATION genActual = m_pHistCacheItemWork->GetHistGenCounter( );

    assert( genDemanded != genActual );
    assert( genDemanded < m_GenCmdList.GetCmdListSize( ) ); //TODO: find clean solution if max number of generations reached. 
	assert( m_GenCmdList[ 0 ].IsCachedGeneration( ) );      // at least initial generation is cached

    HIST_GENERATION genCached   = genDemanded;  // search backwards starting with genDemanded
    BOOL            bMicrosteps = TRUE;
        
    while ( m_GenCmdList[ genCached ].IsNotCachedGeneration( ) )
        --genCached;

    // now we have found a cached generation  

    if ( 
            (( genCached <= genActual ) && ( genActual < genDemanded )) || // cached generation is not better than actual generation
            (( genActual == genDemanded - 1 ) && bMicrosteps )
        )
    {
        step2NextGeneration( m_GenCmdList[ genActual + 1 ] );   // compute forwards
    }
    else  // get cached generation
    {
        m_pHistCacheItemWork->CopyCacheItem( getCachedItem( m_GenCmdList[ genCached ] ) );
    }
}

tGenCmd HistorySystemImpl::GetGenerationCmd( HIST_GENERATION const gen )
{
	GenerationCmd cmd = m_GenCmdList[ gen ];

	if ( cmd.IsCachedGeneration( ) )
	{
		cmd = getCachedItem( cmd )->GetGenCmd( );
	}

	return cmd.GetCommand( );
}

// private member functions

// save2History - Save Generation data (rule how to get to next generation), current Grid, etc. to new slot

void HistorySystemImpl::save2History( )
{
    short         const   sSlotNr         = m_pHistoryCache->GetFreeCacheSlotNr( );
    HistCacheItem const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
    GenerationCmd const   genCmdFromCache = pHistCacheItem->GetGenCmd( );

    if ( genCmdFromCache.IsDefined( ) )       // Hist slot was in use before. Save GenCommand
    {
        HIST_GENERATION const genCached = pHistCacheItem->GetHistGenCounter( );
        assert( m_GenCmdList[ genCached ].IsCachedGeneration( ) );
        assert( m_GenCmdList[ genCached ].GetSlotNr( ) == sSlotNr );
        m_GenCmdList.SetGenerationCmd( genCached, genCmdFromCache );
        m_pHistoryCache->ResetHistCacheSlot( sSlotNr );
    }

    CHECK_HISTORY_STRUCTURE;
    m_pHistoryCache->Save2CacheSlot( * m_pHistCacheItemWork, sSlotNr );
    m_GenCmdList.SetCachedGeneration( m_pHistCacheItemWork->GetHistGenCounter( ), sSlotNr );
    CHECK_HISTORY_STRUCTURE;
};

// step2NextGeneration - if cached generation: get GenerationCmd from cache
//                     - apply command and increment history generation counter

void HistorySystemImpl::step2NextGeneration( GenerationCmd genCmd )
{
    if ( genCmd.IsCachedGeneration( ) ) // can happen only in history mode
    {
        assert( IsInHistoryMode( ) );
        short         const   sSlotNr        = genCmd.GetSlotNr( );
        HistCacheItem const * pHistCacheItem = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
        genCmd = pHistCacheItem->GetGenCmd( );
    }

	m_pHistCacheItemWork->GetModelData()->OnAppCommand( genCmd );    // Apply application defined operation to step to next generation
    m_pHistCacheItemWork->IncHistGenCounter( );

    assert( m_pHistCacheItemWork->GetHistGenCounter( ) < m_GenCmdList.GetCmdListSize( ) ); //TODO: find clean solution if max number of generations reached. 
}

HIST_GENERATION HistorySystemImpl::FindFirstGenerationWithProperty( GenerationProperty const & property ) const
{
    HistoryIterator histIter( m_pHistoryCache );    // we move forwards thru time, starting with oldest generation.

    for ( int iRun = histIter.Set2Oldest( ); iRun != -1; iRun = histIter.Set2Junior( ) )
    {
        if ( (property)( m_pHistoryCache->GetHistCacheItemC( iRun )->GetModelDataC( ) ) )
        {
            return m_pHistoryCache->GetGridGen( iRun );
        }
    }

    return -1;
}

HIST_GENERATION HistorySystemImpl::FindLastGenerationWithProperty( GenerationProperty const & property ) const
{
    HistoryIterator histIter( m_pHistoryCache );    // we move backwards thru time, starting with youngest generation.

    for ( int iRun = histIter.Set2Youngest( ); iRun != -1; iRun = histIter.Set2Senior( ) )
    {
        if ( (property)( m_pHistoryCache->GetHistCacheItemC( iRun )->GetModelDataC( ) ) )
        {
            return m_pHistoryCache->GetGridGen( iRun );
        }
    }

    return -1;
}

void HistorySystemImpl::checkHistoryStructure( )  // used only in debug mode
{
     // check generation number of all generations in history cache

//	wcout << L"**** checkHistoryStructure" << endl;
//	wcout << L"m_genCmdList" << endl;

    for ( HIST_GENERATION gen = 0; gen < m_GenCmdList.GetCmdListSize( ); ++gen )
    {
        GenerationCmd generationCmd = m_GenCmdList[ gen ];
//		if ( generationCmd.IsDefined( ) )
//			wcout << gen 
//			      << L" (" << generationCmd.GetCommand() 
//				  << L"," << generationCmd.GetParam()
//			      << L")";
        if ( generationCmd.IsCachedGeneration( ) )
        {
            short           const   sSlotNrFromList = generationCmd.GetSlotNr( );
            HistCacheItem   const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC( sSlotNrFromList );
            HIST_GENERATION const   genNrFromCache  = pHistCacheItem->GetHistGenCounter( );
	        GenerationCmd   const   genCmdFromCache = pHistCacheItem->GetGenCmd( );
            assert( genNrFromCache == gen );
//			wcout << L" cache: slot "
//				  << sSlotNrFromList
//	              << L" (" << genCmdFromCache.GetCommand() << L"," << genCmdFromCache.GetParam() << L") "
//				  << genNrFromCache << endl;
        }
        else if ( generationCmd.IsUndefined( ) )
            break;
//		else
//			wcout << endl;
    }

    // check slot numbers and according generation commands

//	wcout << L"slots" << L"(" << m_pHistoryCache->GetNrOfHistCacheSlots( ) << L")" << endl;

    for ( short sSlotNr = 0; sSlotNr < m_pHistoryCache->GetNrOfHistCacheSlots( ); ++sSlotNr )
    {
        HistCacheItem   const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
        HIST_GENERATION const   genNrFromCache  = pHistCacheItem->GetHistGenCounter( );
        GenerationCmd   const   genCmdFromCache = pHistCacheItem->GetGenCmd( );
        if ( genCmdFromCache.IsDefined( ) )
        {
            GenerationCmd const generationCmd   = m_GenCmdList[ genNrFromCache ];
            short         const sSlotNrFromList = generationCmd.GetSlotNr( );
//			wcout << L"slot " << sSlotNr 
//				  << L" (" << genCmdFromCache.GetCommand() << L"," << genCmdFromCache.GetParam() << L") "
//				  << L"gen " << genNrFromCache
//				  << L" sSlotNrFromList= " << sSlotNrFromList 
//				  << endl;
            assert( generationCmd.IsCachedGeneration( ) );
            assert( sSlotNrFromList == sSlotNr );
        }
    }
}