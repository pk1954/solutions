// HistoryCache.ixx
//
// History

module;

#include <cassert>
#include <vector>

export module HistoryCache;

import ObserverInterface;
import HistCacheItem;
import HistGeneration;
import HistSlotNr;
import HistSlot;

export class HistoryCache
{
public:

    explicit HistoryCache();
    ~HistoryCache();
    
    void       InitHistoryCache(HistSlotNr const, ModelFactory const * const);
    bool       AddCacheSlot();
    void       ResetHistoryCache();
    HistSlotNr GetFreeCacheSlot();
    void       RemoveHistCacheSlot(HistSlotNr const);

	void HistoryCache::ResetHistCacheSlot(HistSlotNr const slotNr)  	// reset slot, but leave it in list of used slots
	{
		getSlot(slotNr).ResetSlot();
	}

    ModelData const * Save2CacheSlot
	(
		HistSlotNr    const   slotNr,
		HistCacheItem const * pSource
	)
	{
		return GetHistCacheItem(slotNr)->CopyCacheItemFrom(pSource);
	}

    void CopyFromCacheSlot
	(
		HistSlotNr    const slotNr,
		HistCacheItem     * pTarget
	)
	{
        pTarget->CopyCacheItemFrom(GetHistCacheItemC(slotNr));
	}

	bool IsEmpty()    const { return m_histSlotHead.IsNull(); };
    bool IsNotEmpty() const { return m_histSlotHead.IsNotNull(); };

    HistSlotNr GetHead() const { return m_histSlotHead; };
	HistSlotNr GetTail() const { return HistSlotNr{0}; };
	HistSlotNr GetSenior(HistSlotNr slotNr) const { return getSlotC(slotNr).GetSeniorGen(); }
	HistSlotNr GetJunior(HistSlotNr slotNr) const { return getSlotC(slotNr).GetJuniorGen(); }
	
	HistGeneration GetGridGen(HistSlotNr slotNr) const { return getSlotC(slotNr).GetGridGeneration(); }

    HistSlotNr      GetNrOfHistCacheSlots    () const { return m_iNrOfSlots; }
    HistSlotNr      GetNrOfUsedHistCacheSlots() const { return m_iNrOfUsedSlots; }
    HistGeneration GetYoungestGeneration    () const { return IsEmpty() ? -1 : GetGridGen(m_histSlotHead); };
    
    void ShutDownHistCache()
	{ 
		m_aHistSlot.clear();
	};

    HistCacheItem const * HistoryCache::GetHistCacheItemC(HistSlotNr const slotNr) const
	{
		return getSlotC(slotNr).GetHistCacheItemC();
	};

	HistCacheItem * HistoryCache::GetHistCacheItem(HistSlotNr const slotNr)
	{
		return getSlot(slotNr).GetHistCacheItem();
	};

private:
    HistoryCache             (HistoryCache const &);  // noncopyable class 
    HistoryCache & operator= (HistoryCache const &);  // noncopyable class 

	ModelFactory const * m_pModelFactory;
	ObserverInterface  * m_pObserver; 

    std::vector< HistSlot > m_aHistSlot;  // is tail of list

	HistSlot & getSlot(HistSlotNr const slotNr)
	{
		return m_aHistSlot.at(slotNr.GetValue());
	}

	HistSlot const & getSlotC(HistSlotNr const slotNr) const
	{
		return m_aHistSlot.at(slotNr.GetValue());
	}

	void newSlot()
    {
		HistSlot * pNewSlot = new HistSlot(m_pModelFactory);
		m_aHistSlot.push_back(* pNewSlot);
    }

	HistSlotNr m_histSlotHead;     // slot with youngest generation
    HistSlotNr m_iUnused;          // first unused slot
    HistSlotNr m_iStartSearching;  // for optimization

    HistSlotNr m_iNrOfSlots;
    HistSlotNr m_iNrOfRequestedSlots;
    HistSlotNr m_iNrOfUsedSlots;
    
    bool  m_bAllocationRunning;

	void setSenior(HistSlotNr iDst, HistSlotNr iSrc) { getSlot(iDst).SetSeniorGen(iSrc); }
	void setJunior(HistSlotNr iDst, HistSlotNr iSrc) { getSlot(iDst).SetJuniorGen(iSrc); }

	void triggerObserver() 
	{ 	
		if (m_pObserver != nullptr)
			m_pObserver->Notify(false);
	}

    HistSlotNr findSlot4Reuse();
    void       checkConsistency();
};
