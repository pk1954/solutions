// historyIterator.cpp

#include "stdafx.h"
#include "assert.h"
#include "hist_slot.h"
#include "historyCache.h"
#include "historyIterator.h"

HistSlot const * HistoryIterator::Set2Youngest( )
{
    assert( !m_pHistory->IsEmpty( ) );
    return m_phsRun = m_pHistory->GetHead( );
}

HistSlot const * HistoryIterator::Set2Oldest( )
{
    if ( m_pHistory->IsEmpty( ) )
        return nullptr;
    return m_phsRun = m_pHistory->GetTail( );
}

HistSlot const * HistoryIterator::Set2Senior( )
{
    if ( IsOldest( ) )
        return nullptr;
    return m_phsRun = m_phsRun->GetSeniorGenC( );
}

HistSlot const * HistoryIterator::Set2Junior( )
{
    if ( IsYoungest( ) )
        return nullptr;
    return m_phsRun = m_phsRun->GetJuniorGenC( );
}

bool HistoryIterator::IsYoungest( ) const
{
    return m_pHistory->IsEmpty( ) || ( m_phsRun == m_pHistory->GetHead( ) );
}

bool HistoryIterator::IsNotYoungest( ) const
{
    return m_pHistory->IsNotEmpty( ) && ( m_phsRun != m_pHistory->GetHead( ) );
}

bool HistoryIterator::IsOldest( ) const
{
    return m_pHistory->IsEmpty( ) || ( m_phsRun == m_pHistory->GetTail( ) );
}

HistCacheItem const * HistoryIterator::GetCurrentHistCacheItem( ) const
{
    assert( m_phsRun != nullptr );
    return m_phsRun->GetHistCacheItemC( );
}

HIST_GENERATION HistoryIterator::GetCurrentGeneration( ) const
{
    assert( m_phsRun != nullptr );
    return m_phsRun->GetGridGeneration( );
};

HIST_GENERATION HistoryIterator::GetJuniorGeneration( ) const
{
    assert( m_phsRun != nullptr );
    HistSlot const * const phsJunior = m_phsRun->GetJuniorGenC( );
    return ( phsJunior == nullptr ) ? LONG_MAX : phsJunior->GetGridGeneration( );  //TODO: check if safe
};

HIST_GENERATION HistoryIterator::GetSeniorGeneration( ) const
{
    assert( m_phsRun != nullptr );
    HistSlot const * const phsSenior = m_phsRun->GetSeniorGenC( );
    return ( phsSenior == nullptr ) ? 0 : phsSenior->GetGridGeneration( );
};
