// hist_slot.h : 
//

#pragma once

#include "HistCacheItem.h"
#include "HistoryGeneration.h"

class HistCacheItem;

class HistSlot
{
public: 
    HistSlot( ) :
        m_iSenior       ( -1 ),
        m_iJunior       ( -1 ),
        m_pHistCacheItem( nullptr )
    {}

    virtual ~HistSlot( )
    {
        ShutDownHistCacheItem( );
    }

    int GetSeniorGen( ) const { return m_iSenior; }
    int GetJuniorGen( ) const { return m_iJunior; }

    void SetSeniorGen    ( int const iSenior ) { m_iSenior = iSenior; }
    void SetJuniorGen    ( int const iJunior ) { m_iJunior = iJunior; }
    void SetHistCacheItem( HistCacheItem * const pItem   ) { m_pHistCacheItem = pItem;   }
    void ShutDownHistCacheItem( )                                
    { 
        delete m_pHistCacheItem;
        m_pHistCacheItem = nullptr;
    }

    HistCacheItem       * GetHistCacheItem ( ) const { return m_pHistCacheItem; }
    HistCacheItem const * GetHistCacheItemC( ) const { return m_pHistCacheItem; }

    HIST_GENERATION GetGridGeneration( ) const { return m_pHistCacheItem->GetHistGenCounter( ); };

    void ResetSlot( ) { m_pHistCacheItem->InitializeGenCmd( ); }

private:
    int             m_iSenior;
    int             m_iJunior;
    HistCacheItem * m_pHistCacheItem;
};
