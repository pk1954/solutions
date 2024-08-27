// HistoryIterator.ixx
//
// History

export module HistoryLib:HistoryIterator;

import :HistSlotNr;
import :HistoryCache;
import :HistCacheItem;
import :HistGeneration;

export class HistoryIterator
{
public:
    explicit HistoryIterator(HistoryCache const*);

    ~HistoryIterator()
    {
        m_slotNrRun.Set2Null();
    }

    HistSlotNr Set2Youngest();
    HistSlotNr Set2Oldest();
    HistSlotNr Set2Senior();
    HistSlotNr Set2Junior();

    bool IsYoungest()    const;
    bool IsNotYoungest() const;
    bool IsOldest()      const;

    HistCacheItem const* GetCurrentHistCacheItem() const;

    HistGeneration GetCurrentGeneration() const;
    HistGeneration GetJuniorGeneration() const;
    HistGeneration GetSeniorGeneration() const;

private:
    HistoryIterator(HistoryIterator const&); // Disable copy constructor

    HistoryCache const* m_pHistCache;
    HistSlotNr           m_slotNrRun;
};
