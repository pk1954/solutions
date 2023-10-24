// HistSlot.h : 
//

module;

#include "ModelData.h"
#include "HistCacheItem.h"
#include "HistoryGeneration.h"

import HistSlotNr;

class HistSlot
{
public: 

    HistSlot(ModelFactory const * factory) :
        m_histSlotSenior(HistSlotNr::NULL_VAL()),
        m_histSlotJunior(HistSlotNr::NULL_VAL()),
        m_pHistCacheItem(nullptr)
    {
		m_pHistCacheItem = new HistCacheItem(factory);
	}

	~HistSlot()
	{
        delete m_pHistCacheItem;
        m_pHistCacheItem = nullptr;
	}

    HistSlotNr GetSeniorGen() const { return m_histSlotSenior; }
    HistSlotNr GetJuniorGen() const { return m_histSlotJunior; }

    void SetSeniorGen(HistSlotNr const slotNrSenior) { m_histSlotSenior = slotNrSenior; }
    void SetJuniorGen(HistSlotNr const slotNrJunior) { m_histSlotJunior = slotNrJunior; }
    
    HistCacheItem       * GetHistCacheItem () const { return m_pHistCacheItem; }
    HistCacheItem const * GetHistCacheItemC() const { return m_pHistCacheItem; }

    HistGeneration GetGridGeneration() const { return m_pHistCacheItem->GetHistGenCounter(); };

    void ResetSlot() { m_pHistCacheItem->InitializeGenCmd(); }

private:
    HistSlotNr      m_histSlotSenior;
    HistSlotNr      m_histSlotJunior;
    HistCacheItem * m_pHistCacheItem;
};
