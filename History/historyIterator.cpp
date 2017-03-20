// historyIterator.cpp

#include "stdafx.h"
#include "assert.h"
#include "hist_slot.h"
#include "historyCache.h"
#include "historyIterator.h"

int HistoryIterator::Set2Youngest( )
{
    assert( !m_pHistCache->IsEmpty( ) );
    return m_iSlotRun = m_pHistCache->GetHead( );
}

int HistoryIterator::Set2Oldest( )
{
    if ( m_pHistCache->IsEmpty( ) )
        return -1;
    return m_iSlotRun = m_pHistCache->GetTail( );
}

int HistoryIterator::Set2Senior( )
{
    if ( IsOldest( ) )
        return -1;
    return m_iSlotRun = m_pHistCache->GetSenior( m_iSlotRun );
}

int HistoryIterator::Set2Junior( )
{
    if ( IsYoungest( ) )
        return -1;
    return m_iSlotRun = m_pHistCache->GetJunior( m_iSlotRun );
}

bool HistoryIterator::IsYoungest( ) const
{
    return m_pHistCache->IsEmpty( ) || ( m_iSlotRun == m_pHistCache->GetHead( ) );
}

bool HistoryIterator::IsNotYoungest( ) const
{
    return m_pHistCache->IsNotEmpty( ) && ( m_iSlotRun != m_pHistCache->GetHead( ) );
}

bool HistoryIterator::IsOldest( ) const
{
    return m_pHistCache->IsEmpty( ) || ( m_iSlotRun == m_pHistCache->GetTail( ) );
}

HistCacheItem const * HistoryIterator::GetCurrentHistCacheItem( ) const
{
    assert( m_iSlotRun != -1 );
    return m_pHistCache->GetHistCacheItem( m_iSlotRun );
}

HIST_GENERATION HistoryIterator::GetCurrentGeneration( ) const
{
    assert( m_iSlotRun != -1 );
    return m_pHistCache->GetGridGen( m_iSlotRun );
};

HIST_GENERATION HistoryIterator::GetJuniorGeneration( ) const
{
    assert( m_iSlotRun != -1 );
    int const iJunior = m_pHistCache->GetJunior( m_iSlotRun );
    return ( iJunior == -1 ) ? LONG_MAX : m_pHistCache->GetGridGen( iJunior );  //TODO: check if safe
};

HIST_GENERATION HistoryIterator::GetSeniorGeneration( ) const
{
    assert( m_iSlotRun != -1 );
    int const iSenior = m_pHistCache->GetSenior( m_iSlotRun );
    return ( iSenior == -1 ) ? 0 : m_pHistCache->GetGridGen( iSenior );
};
