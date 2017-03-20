// historyIterator.h
//

#pragma once

#include "HistoryGeneration.h"

class HistoryCache;
class HistSlot;
class HistCacheItem;

class HistoryIterator
{
public:
    explicit HistoryIterator( HistoryCache const * pHistCache ) : 
        m_pHistCache( pHistCache ),
        m_iSlotRun( -1 )
    { }

    ~HistoryIterator( )
    {
        m_iSlotRun = -1;
    }

    int Set2Youngest( );
    int Set2Oldest( );
    int Set2Senior( );
    int Set2Junior( );

    bool IsYoungest( )    const;
    bool IsNotYoungest( ) const;
    bool IsOldest( )      const;
    
    HistCacheItem const * GetCurrentHistCacheItem( ) const;

    HIST_GENERATION GetCurrentGeneration( ) const;
    HIST_GENERATION GetJuniorGeneration ( ) const;
    HIST_GENERATION GetSeniorGeneration ( ) const;

private:
    HistoryIterator( HistoryIterator const & ); // Disable copy constructor

    HistoryCache const * m_pHistCache;
    int                  m_iSlotRun;
};
