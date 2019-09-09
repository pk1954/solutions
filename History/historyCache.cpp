// history.cpp

#include "stdafx.h"
#include "assert.h"
#include "historyCache.h"

#ifndef NDEBUG
	#define CHECK_CONSISTENCY() checkConsistency()
#else
	#define CHECK_CONSISTENCY() ((void)0)
#endif

HistoryCache::HistoryCache( ) :
    m_histSlotHead( HistSlotNr::NULL_VAL() ),
    m_iUnused( HistSlotNr::NULL_VAL() ),
    m_iStartSearching( HistSlotNr::NULL_VAL() ),
	m_pModelFactory( nullptr ),
	m_pObserver( nullptr ),
	m_iNrOfSlots( 0 ),
	m_iNrOfUsedSlots( 0 ),
    m_bAllocationRunning( true )
{ }

HistoryCache::~HistoryCache( )
{
    m_histSlotHead.Set2Null();
    m_iUnused.Set2Null();
    m_iStartSearching.Set2Null();

    m_iNrOfSlots     = HistSlotNr(0);
    m_iNrOfUsedSlots = HistSlotNr(0);
};

void HistoryCache::InitHistoryCache
( 
	HistSlotNr           const nrOfSlots, 
	ModelFactory const * const pModelFactory
)
{
	m_pModelFactory = pModelFactory;

    assert( nrOfSlots >= HistSlotNr(2) );

    m_iNrOfRequestedSlots = nrOfSlots;

#ifdef _DEBUG
    if ( m_iNrOfRequestedSlots > HistSlotNr(60) )
        m_iNrOfRequestedSlots = HistSlotNr(60);
#endif
    m_aHistSlot.reserve( m_iNrOfRequestedSlots.GetValue() );
	newSlot( );
    ++m_iNrOfSlots;
	triggerObserver();
    m_iUnused = HistSlotNr(0);
}

void HistoryCache::ResetHistoryCache( )
{
    m_histSlotHead.Set2Null();
    m_iStartSearching.Set2Null();
	m_iUnused         = HistSlotNr(0);
    m_iNrOfUsedSlots  = HistSlotNr(0);

    setSenior( HistSlotNr(0), HistSlotNr::NULL_VAL() );
    setJunior( HistSlotNr(0), HistSlotNr(1) );

    for ( HistSlotNr slotNr = HistSlotNr(1); slotNr < m_iNrOfSlots - HistSlotNr(1); ++slotNr )
    {
        setSenior( slotNr, slotNr - HistSlotNr(1) );
        setJunior( slotNr, slotNr + HistSlotNr(1) );
    }

    setSenior( m_iNrOfSlots - HistSlotNr(1), m_iNrOfSlots );
    setJunior( m_iNrOfSlots - HistSlotNr(1), HistSlotNr::NULL_VAL() );
}

bool HistoryCache::AddCacheSlot( )
{
	try
    {
		newSlot( );
    }
    catch ( std::bad_alloc & )
    {
        m_bAllocationRunning = false;
    }

    if ( m_bAllocationRunning )
    {
        if ( m_iUnused.IsNull() )
            m_iUnused = m_iNrOfSlots;

        setSenior( m_iNrOfSlots,                 m_iNrOfSlots - HistSlotNr(1) );
        setJunior( m_iNrOfSlots - HistSlotNr(1), m_iNrOfSlots                 );
        ++m_iNrOfSlots;
		triggerObserver();
		CHECK_CONSISTENCY( );
        m_bAllocationRunning = ( m_iNrOfSlots < m_iNrOfRequestedSlots );
    }

    return m_bAllocationRunning;
}

void HistoryCache::checkConsistency( )
{
    HistSlotNr nrOfUsedSlots  (0);
    HistSlotNr nrOfUnusedSlots(0);

    for ( HistSlotNr slotNr = m_histSlotHead; slotNr.IsNotNull(); slotNr = GetSenior( slotNr ) )
    {
        ++nrOfUsedSlots;
        assert( nrOfUsedSlots <= m_iNrOfSlots );
    }

    for ( HistSlotNr slotNr = m_iUnused; slotNr.IsNotNull(); slotNr = GetJunior( slotNr ) )
    {
        ++nrOfUnusedSlots;
        assert( nrOfUnusedSlots <= m_iNrOfSlots );
    }

    assert( nrOfUsedSlots == m_iNrOfUsedSlots );
    assert( nrOfUsedSlots + nrOfUnusedSlots == m_iNrOfSlots );
}

HistSlotNr HistoryCache::GetFreeCacheSlot( )
{
    static long lSleepCounter = 0;

	CHECK_CONSISTENCY( );

    while ( m_bAllocationRunning && ( m_iNrOfSlots <= m_iNrOfUsedSlots ) )  // possible race condition during start up
    {                                                                       // slot usage might be faster than slot creation
        Sleep( 10 );                                                        // wait until slots are created
        ++lSleepCounter; 
    }

    HistSlotNr const slotNrSenior = m_histSlotHead;

    if ( m_iUnused.IsNotNull() )         // Unused slots available
    {
        assert( m_iNrOfUsedSlots < m_iNrOfSlots );
        m_histSlotHead = m_iUnused;
        m_iUnused = GetJunior( m_iUnused );
        ++m_iNrOfUsedSlots;
		triggerObserver();
    }
    else                                // No unused slots. We have to reuse slots
    {
        assert( m_iNrOfUsedSlots == m_iNrOfSlots );
        m_histSlotHead = findSlot4Reuse( );

        HistSlotNr slotNrJunior = GetJunior( m_histSlotHead );
		HistSlotNr slotNrSenior = GetSenior( m_histSlotHead );

		setSenior( slotNrJunior, slotNrSenior );   // remove from list
		setJunior( slotNrSenior, slotNrJunior );   // of used slots
    }

    if ( slotNrSenior.IsNotNull() )               // add to head of list
        setJunior( slotNrSenior, m_histSlotHead );

    setJunior( m_histSlotHead, HistSlotNr::NULL_VAL() );
    setSenior( m_histSlotHead, slotNrSenior );

    return  m_histSlotHead;
}

// RemoveHistCacheSlot
// remove slot from list of used history cache slots

void HistoryCache::RemoveHistCacheSlot( HistSlotNr const slotNr )
{
    HistSlotNr slotNrJunior = GetJunior( slotNr );
    HistSlotNr slotNrSenior = GetSenior( slotNr );

    // remove from list of used slots

    if ( m_histSlotHead == slotNr )
        m_histSlotHead =  GetSenior( m_histSlotHead );

    if ( slotNrJunior.IsNotNull() )
        setSenior( slotNrJunior, slotNrSenior );

    if ( slotNrSenior.IsNotNull() )
        setJunior( slotNrSenior, slotNrJunior );

    // add to list of unused slots
    if ( m_iUnused.IsNotNull() )
        setSenior( m_iUnused, slotNr );

    setJunior( slotNr, m_iUnused );
    m_iUnused = slotNr;
    setSenior( m_iUnused, HistSlotNr::NULL_VAL() );

    if ( m_iStartSearching == slotNr )
        m_iStartSearching.Set2Null();

    ResetHistCacheSlot( slotNr );
    --m_iNrOfUsedSlots;
	triggerObserver();
	CHECK_CONSISTENCY( );
}

HistSlotNr HistoryCache::findSlot4Reuse( )
{
    HistSlotNr candidate = m_histSlotHead;

    if ( m_iStartSearching.IsNull() )  // happens only once
    {
        assert( candidate.IsNotNull() );
        candidate = GetSenior( candidate );
        candidate = GetSenior( candidate );
        candidate = GetSenior( candidate );
    }
    else // Go 2 steps forward (if possible) to ensure that iCandidate->m_histSlotSenior->m_histSlotSenior exists
    {
        if ( GetJunior( m_iStartSearching ).IsNotNull() )
            m_iStartSearching = GetJunior( m_iStartSearching );

        if ( GetJunior( m_iStartSearching ).IsNotNull() )
            m_iStartSearching = GetJunior( m_iStartSearching );

        candidate = GetSenior( m_iStartSearching );
    }

    while ( true )
    {
		HistSlotNr slotNrJunior       = GetJunior( candidate );
		HistSlotNr slotNrSenior       = GetSenior( candidate );
		HistSlotNr slotNrSeniorSenior = GetSenior( slotNrSenior );
		
		if ( slotNrSeniorSenior.IsNull() )
			break;

		HIST_GENERATION genJunior       = GetGridGen( slotNrJunior       );
		HIST_GENERATION genSenior       = GetGridGen( slotNrSenior       );
		HIST_GENERATION genSeniorSenior = GetGridGen( slotNrSeniorSenior );

        HIST_GENERATION const thisDiff = genJunior - genSenior;
        HIST_GENERATION const lastDiff = genSenior - genSeniorSenior;

        if ( lastDiff == 1 )
            m_iStartSearching = candidate;

        if ( thisDiff + 1 <= lastDiff )
            break;

        candidate = slotNrSenior;
    }

    return candidate;
}
