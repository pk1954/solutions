// historyCache.h : 
//

#pragma once

#include "stdafx.h"
#include "assert.h"
#include <exception>

#ifdef _DEBUG
#include <iostream>
using namespace std;
#endif

#include "BasicHistCacheItem.h"
#include "HistoryGeneration.h"
#include "hist_slot.h"

//lint -esym( 1565, HistoryCache::m_ulSlotSize )
//lint -esym( 1565, HistoryCache::m_aHistSlot )
//lint -esym( 1565, HistoryCache::m_usNrOfSlots )
//lint -sem(HistoryCache::ResetHistoryCache,initializer)

class HistoryCache
{
public:

    explicit HistoryCache( );
    ~HistoryCache( );
    
    void InitHistoryCache( short const, BasicHistCacheItem * );
    bool AddCacheSlot( BasicHistCacheItem * );
    void ResetHistoryCache( );

    BasicHistCacheItem       * GetHistCacheItem ( short const ) const;
    BasicHistCacheItem const * GetHistCacheItemC( short const ) const;

    short GetFreeCacheSlotNr( );

    void Save2CacheSlot( BasicHistCacheItem const &, short const );
    void RemoveHistCacheSlot( short const );
    void ResetHistCacheSlot ( short const );

    bool IsEmpty( )    const { return m_pHead == nullptr; };
    bool IsNotEmpty( ) const { return m_pHead != nullptr; };

    HistSlot const * GetHead( ) const { return m_pHead; };
    HistSlot const * GetTail( ) const { return m_aHistSlot; };

    short           GetNrOfHistCacheSlots( ) const { return m_sNrOfSlots; }
    HIST_GENERATION GetYoungestGeneration( ) const { return IsEmpty( ) ? -1 : m_pHead->GetGridGeneration( ); };
    
    void ShutDownHistCacheSlot( short const i ) { m_aHistSlot[ i ].ShutDownHistCacheItem( ); };

private:
    HistoryCache             ( HistoryCache const & );  // noncopyable class 
    HistoryCache & operator= ( HistoryCache const & );  // noncopyable class 

    HistSlot * m_aHistSlot;        // is tail of list

    HistSlot * m_pHead;            // slot with youngest generation
    HistSlot * m_pUnused;          // first unused slot
    HistSlot * m_pStartSearching;  // for optimization

    short m_sNrOfSlots;
    short m_sNrOfRequestedSlots;
    short m_sNrOfUsedSlots;
    
    bool       m_bAllocationRunning;

    HistSlot * findSlot4Reuse( );
 
    void checkConsistency( );
};
