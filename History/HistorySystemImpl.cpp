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
#include "genCmdList.h"

// public member functions

HistorySystemImpl::HistorySystemImpl( ) :
    m_pGenCmdList( nullptr ),
    m_pHistoryCache( nullptr ),
    m_pHistCacheItemWork( nullptr ),
    m_pAskHistoryCutFunctor( nullptr ),
    m_pModelDataWork( nullptr )
{ }

HistorySystemImpl::~HistorySystemImpl( )
{
    delete m_pGenCmdList;
    delete m_pHistoryCache;

    m_pGenCmdList = nullptr;
    m_pHistoryCache = nullptr;
    m_pHistCacheItemWork = nullptr;
    m_pAskHistoryCutFunctor = nullptr;
    m_pModelDataWork = nullptr;
}

void HistorySystemImpl::InitHistorySystem
(
    short           const sNrOfSlots,
    HIST_GENERATION const genMaxNrOfGens,
    ModelData     * const pModelDataWork,
    ModelFactory  * const pModelFactory
)
{
    m_pModelDataWork     = pModelDataWork;
	m_pModelFactory      = pModelFactory;
    m_pGenCmdList        = new GenCmdList( genMaxNrOfGens );
    m_pHistoryCache      = new HistoryCache;
	m_pHistCacheItemWork = HistCacheItem::CreateItem( pModelFactory );

    m_pHistoryCache->InitHistoryCache( sNrOfSlots, pModelFactory );
    m_pHistCacheItemWork->SetGenerationCommand( GenerationCmd::RESET );
    save2History( * m_pHistCacheItemWork );
}

bool HistorySystemImpl::AddHistorySlot( ) const 
{ 
    return m_pHistoryCache->AddCacheSlot( m_pHistCacheItemWork, m_pModelFactory );
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
		if  (m_pGenCmdList->IsCachedGeneration( gen ) )
			m_pHistoryCache->RemoveHistCacheSlot( ( * m_pGenCmdList)[gen].GetParam( ) );

		m_pGenCmdList->ResetGenerationCmd(gen);
	}
}

void HistorySystemImpl::createNewGen( GenerationCmd genCmd )
{
	CHECK_HISTORY_STRUCTURE;
    step2NextGeneration( genCmd );
    m_pHistCacheItemWork->SetGenerationCommand( genCmd );
    save2History( * m_pHistCacheItemWork );
    CHECK_HISTORY_STRUCTURE;
}

void HistorySystemImpl::CreateAppCommand( unsigned short const uiCmd, short const sParam )
{
	ClearHistory( GetCurrentGeneration( ) );
	createNewGen( GenerationCmd( uiCmd, sParam ) );
}

// ApproachHistGen - Get closer to demanded HIST_GENERATION
//                 - If several steps are neccessary, function returns after one displayed generation
//                   to allow user interaction
//                 - But actual history generation as alterered by at least 1

void HistorySystemImpl::ApproachHistGen( HIST_GENERATION const genDemanded )
{
    HIST_GENERATION genActual = m_pHistCacheItemWork->GetHistGenCounter( );

    assert( genDemanded != genActual );
    assert( genDemanded < m_pGenCmdList->GetCmdListSize( ) ); //TODO: find clean solution if max number of generations reached. 

    if ( ( * m_pGenCmdList )[ genDemanded ].IsUndefined( ) )   // normal forward mode
    {
        assert( genDemanded == genActual + 1 );
        createNewGen( GenerationCmd::NEXT_GEN );                           
    }
    else   // genDemanded is somewhere in history
    {
        HIST_GENERATION genCached   = genDemanded;  // search backwards starting with genDemanded
        BOOL            bMicrosteps = TRUE;
        
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
            GenerationCmd         const genCmd         = ( * m_pGenCmdList )[ genCached ];
            short                 const sSlotNr        = genCmd.GetParam( );
            HistCacheItem const * const pHistCacheItem = m_pHistoryCache->GetHistCacheItemC( sSlotNr );
            m_pHistCacheItemWork->CopyCacheItem( pHistCacheItem );
        }
    }
}

// private member functions

// save2History - Save Generation data (rule how to get to next generation), current Grid, etc. to new slot

void HistorySystemImpl::save2History( HistCacheItem const & histCacheItem )
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

void HistorySystemImpl::step2NextGeneration( GenerationCmd genCmd )
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
		m_pModelDataWork->OnNextGeneration( );
		break;

	case tGenCmd::reset:
	    m_pModelDataWork->OnReset( );
		break;

	default:
        m_pModelDataWork->OnAppCommand   // Apply application defined operation to step to next generation
		( 
			genCmd.GetAppCmd( ),
			genCmd.GetParam( ) 
		); 
	}

    m_pHistCacheItemWork->IncHistGenCounter( );

    assert( m_pHistCacheItemWork->GetHistGenCounter( ) < m_pGenCmdList->GetCmdListSize( ) ); //TODO: find clean solution if max number of generations reached. 
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
