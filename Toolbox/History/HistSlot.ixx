// HistSlot.ixx
//
// History

export module HistoryLib:HistSlot;

import std;
import :HistCacheItem;
import :HistGeneration;
import :HistSlotNr;
import :ModelData;

using std::unique_ptr;
using std::make_unique;
using std::move;

export class HistSlot
{
public: 

    HistSlot(ModelFactory const &factory) :
        m_histSlotSenior(HistSlotNr::NULL_VAL()),
        m_histSlotJunior(HistSlotNr::NULL_VAL())
    {
		m_upHistCacheItem = move(make_unique<HistCacheItem>(factory));
	}

	~HistSlot()	{}
 
    HistSlot(const HistSlot&)            = delete; // copy constructor
    HistSlot& operator=(const HistSlot&) = delete; // copy assignment

    HistSlot(HistSlot&& other) noexcept 
      : m_histSlotSenior(other.m_histSlotSenior),
        m_histSlotJunior(other.m_histSlotJunior),
        m_upHistCacheItem(move(other.m_upHistCacheItem))
    {}

    HistSlot& operator=(HistSlot&& other) noexcept // move assignment
    {
        m_histSlotSenior  = other.m_histSlotSenior;
        m_histSlotJunior  = other.m_histSlotJunior;
        m_upHistCacheItem = move(other.m_upHistCacheItem);
    }

    HistSlotNr GetSeniorGen() const { return m_histSlotSenior; }
    HistSlotNr GetJuniorGen() const { return m_histSlotJunior; }

    void SetSeniorGen(HistSlotNr const slotNrSenior) { m_histSlotSenior = slotNrSenior; }
    void SetJuniorGen(HistSlotNr const slotNrJunior) { m_histSlotJunior = slotNrJunior; }
    
    HistCacheItem       * GetHistCacheItem ()       { return m_upHistCacheItem.get(); }
    HistCacheItem const * GetHistCacheItemC() const { return m_upHistCacheItem.get(); }

    HistGeneration GetGridGeneration() const { return m_upHistCacheItem->GetHistGenCounter(); };

    void ResetSlot() { m_upHistCacheItem->InitializeGenCmd(); }

private:
    HistSlotNr m_histSlotSenior;
    HistSlotNr m_histSlotJunior;

    unique_ptr<HistCacheItem> m_upHistCacheItem;
};
