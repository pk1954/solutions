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
    m_iHead( HistSlot::NUL ),
    m_iUnused( HistSlot::NUL ),
    m_iStartSearching( HistSlot::NUL ),
    m_iNrOfSlots( 0 ),
    m_iNrOfUsedSlots( 0 ),
    m_bAllocationRunning( true )
{ }

HistoryCache::~HistoryCache( )
{
    m_iHead = HistSlot::NUL;
    m_iUnused = HistSlot::NUL;
    m_iStartSearching = HistSlot::NUL;

    m_iNrOfSlots = 0;
    m_iNrOfUsedSlots = 0;
};

void HistoryCache::InitHistoryCache
( 
	short                const sNrOfSlots, 
	ModelFactory const * const pModelFactory
)
{
	m_pModelFactory = pModelFactory;

	HistCacheItem * pNewHistCacheItem = HistCacheItem::CreateItem( m_pModelFactory );

    assert( sNrOfSlots >= 2 );

    m_iNrOfRequestedSlots = sNrOfSlots;

#ifdef _DEBUG
    if ( m_iNrOfRequestedSlots > 60 )
        m_iNrOfRequestedSlots = 60;
#endif
    m_aHistSlot.resize( m_iNrOfRequestedSlots );

    m_aHistSlot[ 0 ].SetHistCacheItem( pNewHistCacheItem );
    ++m_iNrOfSlots;

    m_iUnused = 0;
}

void HistoryCache::ResetHistoryCache( )
{
    m_iHead           = HistSlot::NUL;
    m_iStartSearching = HistSlot::NUL;
    m_iUnused         =  0;
    m_iNrOfUsedSlots  =  0;

    setSenior( 0, HistSlot::NUL );
    setJunior( 0,  1 );

    for ( int iRun = 1; iRun < m_iNrOfSlots - 1; ++iRun )
    {
        setSenior( iRun, iRun - 1 );
        setJunior( iRun, iRun + 1 );
    }

    setSenior( m_iNrOfSlots - 1, m_iNrOfSlots );
    setJunior( m_iNrOfSlots - 1,           HistSlot::NUL );
}

bool HistoryCache::AddCacheSlot( )
{
    try
    {
		HistCacheItem * pHistCacheItemNew = HistCacheItem::CreateItem( m_pModelFactory );
        m_aHistSlot.at( m_iNrOfSlots ).SetHistCacheItem( pHistCacheItemNew );
    }
    catch ( std::bad_alloc & )
    {
        m_bAllocationRunning = false;
    }

    if ( m_bAllocationRunning )
    {
        if ( m_iUnused == HistSlot::NUL )
            m_iUnused = m_iNrOfSlots;

        setSenior( m_iNrOfSlots,     m_iNrOfSlots - 1 );
        setJunior( m_iNrOfSlots - 1, m_iNrOfSlots     );
        ++m_iNrOfSlots;

        checkConsistency( );

        m_bAllocationRunning = ( m_iNrOfSlots < m_iNrOfRequestedSlots );
    }

    return m_bAllocationRunning;
}

void HistoryCache::checkConsistency( )
{
    HIST_GENERATION genNrOfUsedSlots = 0;
    HIST_GENERATION genNrOfUnusedSlots = 0;

    for ( int iRun = m_iHead; iRun != HistSlot::NUL; iRun = GetSenior( iRun ) )
    {
        ++genNrOfUsedSlots;
        assert( genNrOfUsedSlots <= m_iNrOfSlots );
    }

    for ( int iRun = m_iUnused; iRun != HistSlot::NUL; iRun = GetJunior( iRun ) )
    {
        ++genNrOfUnusedSlots;
        assert( genNrOfUnusedSlots <= m_iNrOfSlots );
    }

    assert( genNrOfUsedSlots == m_iNrOfUsedSlots );
    assert( genNrOfUsedSlots + genNrOfUnusedSlots == m_iNrOfSlots );
}

short HistoryCache::GetFreeCacheSlotNr( )
{
    static long lSleepCounter = 0;

    checkConsistency( );

    while ( m_bAllocationRunning && ( m_iNrOfSlots <= m_iNrOfUsedSlots ) )  // possible race condition during start up
    {                                                                       // slot usage might be faster than slot creation
        Sleep( 10 );                                                        // wait until slots are created
        ++lSleepCounter; 
    }

    int const iSenior = m_iHead;

    if ( m_iUnused != HistSlot::NUL )         // Unused slots available
    {
        assert( m_iNrOfUsedSlots < m_iNrOfSlots );
        m_iHead = m_iUnused;
        m_iUnused = GetJunior( m_iUnused );
        ++m_iNrOfUsedSlots;
    }
    else                                // No unused slots. We have to reuse slots
    {
        assert( m_iNrOfUsedSlots == m_iNrOfSlots );
        m_iHead = findSlot4Reuse( );

        int iJunior = GetJunior( m_iHead );
		int iSenior = GetSenior( m_iHead );

		setSenior( iJunior, iSenior );   // remove from list
		setJunior( iSenior, iJunior );   // of used slots
    }

    if ( iSenior != HistSlot::NUL )               // add to head of list
        setJunior( iSenior, m_iHead );

    setJunior( m_iHead, HistSlot::NUL );
    setSenior( m_iHead, iSenior );

    return  m_iHead;
}

// RemoveHistCacheSlot
// remove slot from list of used history cache slots

void HistoryCache::RemoveHistCacheSlot( int const iSlotNr )
{
    int iJunior = GetJunior( iSlotNr );
    int iSenior = GetSenior( iSlotNr );

    // remove from list of used slots

    if ( m_iHead == iSlotNr )
        m_iHead =  GetSenior( m_iHead );

    if ( iJunior != HistSlot::NUL )
        setSenior( iJunior, iSenior );

    if ( iSenior != HistSlot::NUL )
        setJunior( iSenior, iJunior );

    // add to list of unused slots
    if ( m_iUnused != HistSlot::NUL )
        setSenior( m_iUnused, iSlotNr );

    setJunior( iSlotNr, m_iUnused );
    m_iUnused = iSlotNr;
    setSenior( m_iUnused, HistSlot::NUL );

    if ( m_iStartSearching == iSlotNr )
        m_iStartSearching = HistSlot::NUL;

    ResetHistCacheSlot( iSlotNr );
    --m_iNrOfUsedSlots;

    checkConsistency( );
}

void HistoryCache::Save2CacheSlot
(
    HistCacheItem const & source,
    short         const   iSlotNr
)
{
    GetHistCacheItem( iSlotNr )->CopyCacheItem( & source );
}

int HistoryCache::findSlot4Reuse( )
{
    int iCandidate = m_iHead;

    if ( m_iStartSearching == HistSlot::NUL )  // happens only once
    {
        assert( iCandidate != HistSlot::NUL );
        iCandidate = GetSenior( iCandidate );
        iCandidate = GetSenior( iCandidate );
        iCandidate = GetSenior( iCandidate );
    }
    else // Go 2 steps forward (if possible) to ensure that iCandidate->m_iSenior->m_iSenior exists
    {
        if ( GetJunior( m_iStartSearching ) != HistSlot::NUL )
            m_iStartSearching = GetJunior( m_iStartSearching );

        if ( GetJunior( m_iStartSearching ) != HistSlot::NUL )
            m_iStartSearching = GetJunior( m_iStartSearching );

        iCandidate = GetSenior( m_iStartSearching );
    }

    while ( true )
    {
		int iJunior       = GetJunior( iCandidate );
		int iSenior       = GetSenior( iCandidate );
		int iSeniorSenior = GetSenior( iSenior );
		
		if ( iSeniorSenior == HistSlot::NUL )
			break;

		HIST_GENERATION genJunior       = GetGridGen( iJunior       );
		HIST_GENERATION genSenior       = GetGridGen( iSenior       );
		HIST_GENERATION genSeniorSenior = GetGridGen( iSeniorSenior );

        HIST_GENERATION const thisDiff = genJunior - genSenior;
        HIST_GENERATION const lastDiff = genSenior - genSeniorSenior;

        if ( lastDiff == 1 )
            m_iStartSearching = iCandidate;

        if ( thisDiff + 1 <= lastDiff )
            break;

        iCandidate = iSenior;
    }

    return iCandidate;
}
