// historyCache.h : 
//

#pragma once

#include "stdafx.h"
#include "assert.h"
#include <vector>
#include "observerInterface.h"
#include "HistCacheItem.h"
#include "HistoryGeneration.h"
#include "hist_slot.h"

class ModelFactory;

class HistoryCache
{
public:

    explicit HistoryCache( );
    ~HistoryCache( );
    
    void InitHistoryCache( short const, ModelFactory const * const, ObserverInterface * const );
    bool AddCacheSlot( );
    void ResetHistoryCache( );
    short GetFreeCacheSlotNr( );
    void Save2CacheSlot( HistCacheItem const &, short const );
    void RemoveHistCacheSlot( int const );

	void HistoryCache::ResetHistCacheSlot( int const iSlotNr )  	// reset slot, but leave it in list of used slots
	{
		m_aHistSlot.at( iSlotNr ).ResetSlot( );
	}

    bool IsEmpty( )    const { return m_iHead == -1; };
    bool IsNotEmpty( ) const { return m_iHead != -1; };

    int GetHead( ) const { return m_iHead; };
    int GetTail( ) const { return 0; };
	int GetSenior( int iSlot ) const { return m_aHistSlot.at( iSlot ).GetSeniorGen( ); }
	int GetJunior( int iSlot ) const { return m_aHistSlot.at( iSlot ).GetJuniorGen( ); }
	
	HIST_GENERATION GetGridGen( int iSlot ) const { return m_aHistSlot.at( iSlot ).GetGridGeneration( ); }

    short           GetNrOfHistCacheSlots    ( ) const { return m_iNrOfSlots; }
    short           GetNrOfUsedHistCacheSlots( ) const { return m_iNrOfUsedSlots; }
    HIST_GENERATION GetYoungestGeneration    ( ) const { return IsEmpty( ) ? -1 : GetGridGen( m_iHead ); };
    
    void ShutDownHistCacheSlot( short const iSlotNr )
	{ 
		m_aHistSlot.at( iSlotNr ).ShutDownHistCacheItem( ); 
	};

    HistCacheItem const * HistoryCache::GetHistCacheItemC( int const iSlotNr ) const
	{
		return m_aHistSlot.at( iSlotNr ).GetHistCacheItemC( );
	};

	HistCacheItem * HistoryCache::GetHistCacheItem( int const iSlotNr ) const
	{
		return m_aHistSlot.at( iSlotNr ).GetHistCacheItem( );
	};

private:
    HistoryCache             ( HistoryCache const & );  // noncopyable class 
    HistoryCache & operator= ( HistoryCache const & );  // noncopyable class 

	ModelFactory const * m_pModelFactory;
	ObserverInterface  * m_pObserver; 

    std::vector< HistSlot > m_aHistSlot;  // is tail of list

	int m_iHead;            // slot with youngest generation
    int m_iUnused;          // first unused slot
    int m_iStartSearching;  // for optimization

    int m_iNrOfSlots;
    int m_iNrOfRequestedSlots;
    int m_iNrOfUsedSlots;
    
    bool  m_bAllocationRunning;

	void setSenior( int iDst, int iSrc ) { m_aHistSlot.at( iDst ).SetSeniorGen( iSrc ); }
	void setJunior( int iDst, int iSrc ) { m_aHistSlot.at( iDst ).SetJuniorGen( iSrc ); }

	void triggerObserver() 
	{ 	
		if ( m_pObserver != nullptr )
			m_pObserver->Notify( false );
	}

    int  findSlot4Reuse( );
    void checkConsistency( );
};
