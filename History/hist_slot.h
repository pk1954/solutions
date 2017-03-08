// hist_slot.h : 
//

#pragma once

#include "BasicHistCacheItem.h"
#include "HistoryGeneration.h"

class BasicHistCacheItem;

class HistSlot
{
public: 
    HistSlot( ) :
        m_pSenior       ( nullptr ),
        m_pJunior       ( nullptr ),
        m_pHistCacheItem( nullptr )
    {}

    virtual ~HistSlot( )
    {
        ShutDownHistCacheItem( );
    }

    HistSlot const * GetSeniorGenC( ) const { return m_pSenior; }
    HistSlot const * GetJuniorGenC( ) const { return m_pJunior; }

    HistSlot * GetSeniorGen( ) { return m_pSenior; }
    HistSlot * GetJuniorGen( ) { return m_pJunior; }

    void SetSeniorGen    ( HistSlot           * const pSenior ) { m_pSenior        = pSenior; }
    void SetJuniorGen    ( HistSlot           * const pJunior ) { m_pJunior        = pJunior; }
    void SetHistCacheItem( BasicHistCacheItem * const pItem   ) { m_pHistCacheItem = pItem;   }
    void ShutDownHistCacheItem( )                                
    { 
        delete m_pHistCacheItem;
        m_pHistCacheItem = nullptr;
    }

    BasicHistCacheItem       * GetHistCacheItem ( ) const { return m_pHistCacheItem; }
    BasicHistCacheItem const * GetHistCacheItemC( ) const { return m_pHistCacheItem; }

    HIST_GENERATION GetGridGeneration( ) const { return m_pHistCacheItem->GetHistGenCounter( ); };

    void ResetSlot( ) { m_pHistCacheItem->InitializeGenCmd( ); }

private:
    HistSlot           * m_pSenior;
    HistSlot           * m_pJunior;
    BasicHistCacheItem * m_pHistCacheItem;
};
