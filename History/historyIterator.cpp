// historyIterator.cpp

#include "stdafx.h"
#include "assert.h"
#include "hist_slot.h"
#include "historyCache.h"
#include "historyIterator.h"

HistoryIterator::HistoryIterator( HistoryCache const * pHistCache ) : 
    m_pHistCache( pHistCache ),
    m_slotNrRun( HistSlotNr::NULL_VAL() )
{ }

HistSlotNr HistoryIterator::Set2Youngest( )
{
    assert( !m_pHistCache->IsEmpty( ) );
    return m_slotNrRun = m_pHistCache->GetHead( );
}

HistSlotNr HistoryIterator::Set2Oldest( )
{
    if ( m_pHistCache->IsEmpty( ) )
        return HistSlotNr::NULL_VAL();
    return m_slotNrRun = m_pHistCache->GetTail( );
}

HistSlotNr HistoryIterator::Set2Senior( )
{
    if ( IsOldest( ) )
        return HistSlotNr::NULL_VAL();
    return m_slotNrRun = m_pHistCache->GetSenior( m_slotNrRun );
}

HistSlotNr HistoryIterator::Set2Junior( )
{
    if ( IsYoungest( ) )
        return HistSlotNr::NULL_VAL();
    return m_slotNrRun = m_pHistCache->GetJunior( m_slotNrRun );
}

bool HistoryIterator::IsYoungest( ) const
{
    return m_pHistCache->IsEmpty( ) || ( m_slotNrRun == m_pHistCache->GetHead( ) );
}

bool HistoryIterator::IsNotYoungest( ) const
{
    return m_pHistCache->IsNotEmpty( ) && ( m_slotNrRun != m_pHistCache->GetHead( ) );
}

bool HistoryIterator::IsOldest( ) const
{
    return m_pHistCache->IsEmpty( ) || ( m_slotNrRun == m_pHistCache->GetTail( ) );
}

HistCacheItem const * HistoryIterator::GetCurrentHistCacheItem( ) const
{
    assert( m_slotNrRun.IsNotNull() );
    return m_pHistCache->GetHistCacheItemC( m_slotNrRun );
}

HIST_GENERATION HistoryIterator::GetCurrentGeneration( ) const
{
    assert( m_slotNrRun.IsNotNull() );
    return m_pHistCache->GetGridGen( m_slotNrRun );
};

HIST_GENERATION HistoryIterator::GetJuniorGeneration( ) const
{
    assert( m_slotNrRun.IsNotNull() );
    HistSlotNr const slotNrJunior = m_pHistCache->GetJunior( m_slotNrRun );
    return ( slotNrJunior.IsNull() ) ? LONG_MAX : m_pHistCache->GetGridGen( slotNrJunior );  //TODO: check if safe
};

HIST_GENERATION HistoryIterator::GetSeniorGeneration( ) const
{
    assert( m_slotNrRun.IsNotNull() );
    HistSlotNr const slotNrSenior = m_pHistCache->GetSenior( m_slotNrRun );
    return ( slotNrSenior.IsNull() ) ? 0 : m_pHistCache->GetGridGen( slotNrSenior );
};
