// hist_slot.h : 
//

#pragma once

#include "ModelData.h"
#include "HistSlotNr.h"
#include "HistCacheItem.h"
#include "HistoryGeneration.h"

class HistSlot
{
public: 

    HistSlot( ) :
        m_histSlotSenior( HistSlotNr::NULL_VAL() ),
        m_histSlotJunior( HistSlotNr::NULL_VAL() ),
        m_pHistCacheItem( nullptr )
    {
	}

    HistSlotNr GetSeniorGen( ) const { return m_histSlotSenior; }
    HistSlotNr GetJuniorGen( ) const { return m_histSlotJunior; }

    void SetSeniorGen( HistSlotNr const slotNrSenior ) { m_histSlotSenior = slotNrSenior; }
    void SetJuniorGen( HistSlotNr const slotNrJunior ) { m_histSlotJunior = slotNrJunior; }
    
	void SetHistCacheItem( ModelFactory const * factory ) 
	{ 
		m_pHistCacheItem = new HistCacheItem( factory->CreateModelData( ) );
	}

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
    HistSlotNr      m_histSlotSenior;
    HistSlotNr      m_histSlotJunior;
    HistCacheItem * m_pHistCacheItem;
};
