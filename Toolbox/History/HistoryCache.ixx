// HistoryCache.ixx
//
// History

export module HistoryLib:HistoryCache;

import std;
import Debug;
import ObserverInterface;
import :HistCacheItem;
import :HistGeneration;
import :HistSlotNr;
import :HistSlot;
import :ModelData;

using std::vector;
using std::unique_ptr;
using std::make_unique;

export class HistoryCache
{
public:

    explicit HistoryCache();
    ~HistoryCache();
    
    void       InitHistoryCache(HistSlotNr const, ModelFactory const&);
    bool       AddCacheSlot();
    void       ResetHistoryCache();
    HistSlotNr GetFreeCacheSlot();
    void       RemoveHistCacheSlot(HistSlotNr const);

	void ResetHistCacheSlot(HistSlotNr const slotNr)  	// reset slot, but leave it in list of used slots
	{
		getSlot(slotNr).ResetSlot();
	}

    void Save2CacheSlot
	(
		HistSlotNr    const   slotNr,
		HistCacheItem const * pSource
	)
	{
		GetHistCacheItem(slotNr)->CopyCacheItemFrom(pSource);
	}

    void CopyFromCacheSlot
	(
		HistSlotNr const slotNr,
		HistCacheItem  * pTarget
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

    HistSlotNr     GetNrOfHistCacheSlots    () const { return m_iNrOfSlots; }
    HistSlotNr     GetNrOfUsedHistCacheSlots() const { return m_iNrOfUsedSlots; }
    HistGeneration GetYoungestGeneration    () const { return IsEmpty() ? -1 : GetGridGen(m_histSlotHead); };
    
    void ShutDownHistCache()
	{ 
		//m_aHistSlot.clear();
	};

    HistCacheItem const * GetHistCacheItemC(HistSlotNr const slotNr) const
	{
		return getSlotC(slotNr).GetHistCacheItemC();
	};

	HistCacheItem * GetHistCacheItem(HistSlotNr const slotNr)
	{
		return getSlot(slotNr).GetHistCacheItem();
	};

private:
    HistoryCache             (HistoryCache const &);  // noncopyable class 
    HistoryCache & operator= (HistoryCache const &);  // noncopyable class 

	ModelFactory const * m_pModelFactory;
	ObserverInterface  * m_pObserver; 

    vector<HistSlot> m_aHistSlot;  // is tail of list

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
		m_aHistSlot.emplace_back(HistSlot(*m_pModelFactory));
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
