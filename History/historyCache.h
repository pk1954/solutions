// historyCache.h : 
//

#pragma once

#include "stdafx.h"
#include "assert.h"
#include <vector>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

#include "HistCacheItem.h"
#include "HistoryGeneration.h"
#include "hist_slot.h"

class ModelFactory;

class HistoryCache
{
public:

    explicit HistoryCache( );
    ~HistoryCache( );
    
    void InitHistoryCache( short const, ModelFactory const * const );
    bool AddCacheSlot( HistCacheItem *, ModelFactory const * const );
    void ResetHistoryCache( );

    HistCacheItem       * GetHistCacheItem ( short const ) const;
    HistCacheItem const * GetHistCacheItemC( short const ) const;

    short GetFreeCacheSlotNr( );

    void Save2CacheSlot( HistCacheItem const &, short const );
    void RemoveHistCacheSlot( short const );
    void ResetHistCacheSlot ( short const );

    bool IsEmpty( )    const { return m_pHead == nullptr; };
    bool IsNotEmpty( ) const { return m_pHead != nullptr; };

    HistSlot const * GetHead( ) const { return m_pHead; };
    HistSlot const * GetTail( ) const { return & m_aHistSlot[0]; };

    short           GetNrOfHistCacheSlots( ) const { return m_sNrOfSlots; }
    HIST_GENERATION GetYoungestGeneration( ) const { return IsEmpty( ) ? -1 : m_pHead->GetGridGeneration( ); };
    
    void ShutDownHistCacheSlot( short const i ) { m_aHistSlot[ i ].ShutDownHistCacheItem( ); };

private:
    HistoryCache             ( HistoryCache const & );  // noncopyable class 
    HistoryCache & operator= ( HistoryCache const & );  // noncopyable class 

    vector< HistSlot > m_aHistSlot;  // is tail of list

    HistSlot * m_pHead;            // slot with youngest generation
    HistSlot * m_pUnused;          // first unused slot
    HistSlot * m_pStartSearching;  // for optimization

    short m_sNrOfSlots;
    short m_sNrOfRequestedSlots;
    short m_sNrOfUsedSlots;
    
    bool  m_bAllocationRunning;

    HistSlot * findSlot4Reuse( );
 
    void checkConsistency( );
};
