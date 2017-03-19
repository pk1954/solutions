// history.cpp

#include "stdafx.h"
#include "assert.h"
#include <exception>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

#include "HistCacheItem.h"
#include "HistoryGeneration.h"
#include "hist_slot.h"
#include "historyCache.h"

HistoryCache::HistoryCache( ) :
    m_pHead( nullptr ),
    m_pUnused( nullptr ),
    m_pStartSearching( nullptr ),
    m_sNrOfSlots( 0 ),
    m_sNrOfUsedSlots( 0 ),
    m_bAllocationRunning( true )
{ }

HistoryCache::~HistoryCache( )
{
    m_pHead = nullptr;
    m_pUnused = nullptr;
    m_pStartSearching = nullptr;

    m_sNrOfSlots = 0;
    m_sNrOfUsedSlots = 0;
};

void HistoryCache::InitHistoryCache
( 
	short                const sNrOfSlots, 
	ModelFactory const * const pModelFactory
)
{
	HistCacheItem * pNewHistCacheItem = HistCacheItem::CreateItem( pModelFactory );

    assert( sNrOfSlots >= 2 );

    m_sNrOfRequestedSlots = sNrOfSlots;

#ifdef _DEBUG
    if ( m_sNrOfRequestedSlots > 60 )
        m_sNrOfRequestedSlots = 60;
#endif
    m_aHistSlot.resize( m_sNrOfRequestedSlots );

    m_aHistSlot[ 0 ].SetHistCacheItem( pNewHistCacheItem );
    ++m_sNrOfSlots;

    m_pUnused = & m_aHistSlot[0];
}

void HistoryCache::ResetHistoryCache( )
{
    m_pHead = nullptr;
    m_pStartSearching = nullptr;
    m_pUnused = & m_aHistSlot[0];
    m_sNrOfUsedSlots = 0;

    HistSlot * pRun = & m_aHistSlot[0];
    pRun->SetSeniorGen( nullptr );
    pRun->SetJuniorGen( pRun + 1 );

    for ( pRun = & m_aHistSlot[ 1 ]; pRun < & m_aHistSlot[ m_sNrOfSlots - 1 ]; ++pRun )
    {
        pRun->SetSeniorGen( pRun - 1 );
        pRun->SetJuniorGen( pRun + 1 );
    }

    pRun = & m_aHistSlot[ m_sNrOfSlots - 1 ];
    pRun->SetSeniorGen( & m_aHistSlot[ m_sNrOfSlots ] );
    pRun->SetJuniorGen( nullptr );
}

bool HistoryCache::AddCacheSlot
( 
	HistCacheItem      *       pHistCacheItem, 
	ModelFactory const * const pModelFactory 
)
{
    HistSlot * pRun  = & m_aHistSlot[ m_sNrOfSlots ];
    HistSlot * pPrev = & m_aHistSlot[ m_sNrOfSlots - 1 ];

    try
    {
		HistCacheItem * pHistCacheItemNew = pHistCacheItem->CreateItem( pModelFactory );
        pRun->SetHistCacheItem( pHistCacheItemNew );
    }
    catch ( std::bad_alloc & )
    {
        m_bAllocationRunning = false;
    }

    if ( m_bAllocationRunning )
    {
        if ( m_pUnused == nullptr )
            m_pUnused = pRun;

        pRun->SetSeniorGen( pPrev );
        pPrev->SetJuniorGen( pRun );
        ++m_sNrOfSlots;

        checkConsistency( );

        m_bAllocationRunning = ( m_sNrOfSlots < m_sNrOfRequestedSlots );
    }

    return m_bAllocationRunning;
}

void HistoryCache::checkConsistency( )
{
    HIST_GENERATION genNrOfUsedSlots = 0;
    HIST_GENERATION genNrOfUnusedSlots = 0;

    for ( HistSlot * pRun = m_pHead; pRun != nullptr; pRun = pRun->GetSeniorGen( ) )
    {
        ++genNrOfUsedSlots;
        assert( genNrOfUsedSlots <= m_sNrOfSlots );
    }

    for ( HistSlot * pRun = m_pUnused; pRun != nullptr; pRun = pRun->GetJuniorGen( ) )
    {
        ++genNrOfUnusedSlots;
        assert( genNrOfUnusedSlots <= m_sNrOfSlots );
    }

    assert( genNrOfUsedSlots == m_sNrOfUsedSlots );
    assert( genNrOfUsedSlots + genNrOfUnusedSlots == m_sNrOfSlots );
}

HistCacheItem const * HistoryCache::GetHistCacheItemC( short const sSlotNr ) const
{
    return m_aHistSlot[ sSlotNr ].GetHistCacheItemC( );
};

HistCacheItem * HistoryCache::GetHistCacheItem( short const sSlotNr ) const
{
    return m_aHistSlot[ sSlotNr ].GetHistCacheItem( );
};

short HistoryCache::GetFreeCacheSlotNr( )
{
    static long lSleepCounter = 0;

    checkConsistency( );

    while ( m_bAllocationRunning && ( m_sNrOfSlots <= m_sNrOfUsedSlots ) )  // possible race condition during start up
    {                                                                       // slot usage might be faster than slot creation
        Sleep( 10 );                                                        // wait until slots are created
        ++lSleepCounter; 
    }

    HistSlot * const pSenior = m_pHead;

    if ( m_pUnused != nullptr )         // Unused slots available
    {
        assert( m_sNrOfUsedSlots < m_sNrOfSlots );
        m_pHead = m_pUnused;
        m_pUnused = m_pUnused->GetJuniorGen( );
        ++m_sNrOfUsedSlots;
    }
    else                                // No unused slots. We have to reuse slots
    {
        assert( m_sNrOfUsedSlots == m_sNrOfSlots );
        m_pHead = findSlot4Reuse( );
        m_pHead->GetJuniorGen( )->SetSeniorGen( m_pHead->GetSeniorGen( ) );   // remove from list
        m_pHead->GetSeniorGen( )->SetJuniorGen( m_pHead->GetJuniorGen( ) );   // of used slots
    }

    if ( pSenior != nullptr )               // add to head of list
        pSenior->SetJuniorGen( m_pHead );
    m_pHead->SetJuniorGen( nullptr );
    m_pHead->SetSeniorGen( pSenior );

    assert( static_cast<unsigned long>( m_pHead - & m_aHistSlot[ 0 ] ) <= SHRT_MAX );

    return static_cast<short>( m_pHead - & m_aHistSlot[ 0 ] );
}

// RemoveHistCacheSlot
// remove slot from list of used history cache slots

void HistoryCache::RemoveHistCacheSlot( short const sSlotNr )
{
    HistSlot * pHistCacheItem = & m_aHistSlot[ sSlotNr ];

    // remove from list of used slots

    if ( m_pHead == pHistCacheItem )
        m_pHead = m_pHead->GetSeniorGen( );

    HistSlot * pJunior = pHistCacheItem->GetJuniorGen( );
    if ( pJunior != nullptr )
        pJunior->SetSeniorGen( pHistCacheItem->GetSeniorGen( ) );

    HistSlot * pSenior = pHistCacheItem->GetSeniorGen( );
    if ( pSenior != nullptr )
        pSenior->SetJuniorGen( pHistCacheItem->GetJuniorGen( ) );

    // add to list of unused slots
    if ( m_pUnused != nullptr )
        m_pUnused->SetSeniorGen( pHistCacheItem );

    pHistCacheItem->SetJuniorGen( m_pUnused );
    m_pUnused = pHistCacheItem;
    m_pUnused->SetSeniorGen( nullptr );

    if ( m_pStartSearching == pHistCacheItem )
        m_pStartSearching = nullptr;

    pHistCacheItem->ResetSlot( );
    --m_sNrOfUsedSlots;

    checkConsistency( );
}

// ResetHistCacheSlot
// reset slot, but leave it in list of used slots

void HistoryCache::ResetHistCacheSlot( short const sSlotNr )
{
    m_aHistSlot[ sSlotNr ].ResetSlot( );
}

void HistoryCache::Save2CacheSlot
(
    HistCacheItem const & source,
    short              const   sSlotNr
)
{
    m_aHistSlot[ sSlotNr ].GetHistCacheItem( )->CopyCacheItem( & source );
}

HistSlot * HistoryCache::findSlot4Reuse( )
{
    HistSlot * pCandidate = m_pHead;

    if ( m_pStartSearching == nullptr )  // happens only once
    {
        assert( pCandidate != nullptr );
        //lint -esym(613,pCandidate)  possible use of null pointer
        pCandidate = pCandidate->GetSeniorGen( )->GetSeniorGen( )->GetSeniorGen( );
    }
    else // Go 2 steps forward (if possible) to ensure that pCandidate->m_pSenior->m_pSenior exists
    {
        if ( m_pStartSearching->GetJuniorGen( ) != nullptr )
        {
            m_pStartSearching = m_pStartSearching->GetJuniorGen( );

            if ( m_pStartSearching->GetJuniorGen( ) != nullptr )
                m_pStartSearching = m_pStartSearching->GetJuniorGen( );
        }

        pCandidate = m_pStartSearching->GetSeniorGen( );
    }

    while ( pCandidate->GetSeniorGen( )->GetSeniorGen( ) != nullptr )
    {
        HIST_GENERATION const thisDiff = pCandidate->GetJuniorGen( )->GetGridGeneration( ) - pCandidate->GetSeniorGen( )->GetGridGeneration( );
        HIST_GENERATION const lastDiff = pCandidate->GetSeniorGen( )->GetGridGeneration( ) - pCandidate->GetSeniorGen( )->GetSeniorGen( )->GetGridGeneration( );

        if ( lastDiff == 1 )
            m_pStartSearching = pCandidate;

        if ( thisDiff + 1 <= lastDiff )
            break;

        pCandidate = pCandidate->GetSeniorGen( );
    }

    return pCandidate;
}
