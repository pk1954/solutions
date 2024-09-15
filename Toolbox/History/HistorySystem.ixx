// HistorySystem.ixx
//
// History

export module HistoryLib:HistorySystem;

import std;
import Observable;
import :GenerationCmd;
import :GenCmdList;
import :HistGeneration;
import :HistSlotNr;
import :ModelData;
import :HistoryIterator;
import :HistoryCache;
import :HistCacheItem;
import :ModelData;

using std::unique_ptr;
using std::make_unique;
using std::exception;

export class GenerationProperty  // used when searching for generation with certain property
{
public:
	virtual bool operator() (ModelData const*) const = 0;
};

export class HistoryBufferException : public std::exception { };

export class HistorySystem : public Observable
{
public:
    HistorySystem();
    ~HistorySystem();

    virtual ModelData * StartHistorySystem
    (
		HistGeneration     const, 
		long               const, 
		unsigned long long const, 
		ModelFactory     * const
    );

	virtual void           StopHistorySystem();
	virtual bool		   AddHistorySlot();

    virtual HistSlotNr     GetNrOfUsedHistCacheSlots() const { return m_upHistoryCache->GetNrOfUsedHistCacheSlots();    }
    virtual HistSlotNr     GetNrOfHistCacheSlots()     const { return m_upHistoryCache->GetNrOfHistCacheSlots();        }
    virtual HistGeneration GetNrOfGenerations()        const { return m_upHistoryCache->GetYoungestGeneration() + 1;    }
    virtual HistGeneration GetYoungestGeneration()     const { return m_upHistoryCache->GetYoungestGeneration();        }
	virtual HistGeneration GetCurrentGeneration()      const { return m_upHistCacheItemWork->GetHistGenCounter();       }
    virtual size_t         GetSlotSize()               const { return m_upHistCacheItemWork->GetItemSize();             }
	virtual bool           IsInHistoryMode()           const { return GetCurrentGeneration() < GetYoungestGeneration(); };

	virtual void              ShutDownHistCache() { m_upHistoryCache->ShutDownHistCache(); }

	virtual void              CreateAppCommand(GenerationCmd);
    virtual ModelData const * ApproachHistGen (HistGeneration const);
	virtual GenerationCmd     GetGenerationCmd(HistGeneration const);
	virtual ModelData const * GetModelData    (HistGeneration const);
	virtual void              ClearHistory    (HistGeneration const);
	virtual void              ClearAllHistory (); 

	virtual HistoryIterator * CreateHistoryIterator() const;

    virtual HistGeneration   FindGenerationWithProperty(GenerationProperty const &, bool const) const;

	virtual HistoryCache const * GetHistoryCache() const
	{
		return m_upHistoryCache.get();
	}

private:

    GenCmdList                m_GenCmdList;
    unique_ptr<HistoryCache>  m_upHistoryCache;
    unique_ptr<HistCacheItem> m_upHistCacheItemWork;  // The reference item, where history system gets and restores data 

	void save2History();
    void step2NextGeneration(GenerationCmd);
    void checkHistoryStructure();
};
