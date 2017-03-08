// historyIterator.h
//

#pragma once

#include "HistoryGeneration.h"

//lint -esym( 613, HistoryIterator::m_phsRun ) Possible use of null pointer

class HistoryCache;
class HistSlot;
class BasicHistCacheItem;

class HistoryIterator
{
public:
    explicit HistoryIterator( HistoryCache const * pHistory ) : 
        m_pHistory( pHistory ),
        m_phsRun( nullptr )
    { }

    ~HistoryIterator( )
    {
        m_phsRun = nullptr;
    }

    HistSlot const * Set2Youngest( );
    HistSlot const * Set2Oldest( );
    HistSlot const * Set2Senior( );
    HistSlot const * Set2Junior( );

    bool IsYoungest( )    const;
    bool IsNotYoungest( ) const;
    bool IsOldest( )      const;
    
    BasicHistCacheItem const * GetCurrentHistCacheItem( ) const;

    HIST_GENERATION GetCurrentGeneration( ) const;
    HIST_GENERATION GetJuniorGeneration ( ) const;
    HIST_GENERATION GetSeniorGeneration ( ) const;

private:
    HistoryIterator( HistoryIterator const & ); // Disable copy constructor

    HistoryCache const * m_pHistory;
    HistSlot     const * m_phsRun;
};
