// HistorySystemImpl.h
//

module;

#include "HistoryGeneration.h"
#include "HistorySystem.h"
#include "HistoryCache.h"
#include "genCmdList.h"
#include "GenerationCmd.h"

#ifndef NDEBUG
#define CHECK_HISTORY_STRUCTURE checkHistoryStructure()
#else
#define CHECK_HISTORY_STRUCTURE
#endif

class ObserverInterface;
class HistoryIterator;
class HistCacheItem;
class ModelFactory;
class ModelData;

class HistorySystemImpl: public HistorySystem
{
public:
    HistorySystemImpl();
    ~HistorySystemImpl();

    virtual ModelData * StartHistorySystem
    (
		HistGeneration    const, 
		long               const, 
		unsigned long long const, 
		ModelFactory     * const
   );

	virtual void StopHistorySystem();

	virtual bool			  AddHistorySlot();

    virtual HistSlotNr        GetNrOfUsedHistCacheSlots() const { return m_pHistoryCache->GetNrOfUsedHistCacheSlots();    }
    virtual HistSlotNr        GetNrOfHistCacheSlots()     const { return m_pHistoryCache->GetNrOfHistCacheSlots();        }
    virtual HistGeneration   GetNrOfGenerations()        const { return m_pHistoryCache->GetYoungestGeneration() + 1;    }
    virtual HistGeneration   GetYoungestGeneration()     const { return m_pHistoryCache->GetYoungestGeneration();        }
	virtual HistGeneration   GetCurrentGeneration()      const { return m_pHistCacheItemWork->GetHistGenCounter();       }
    virtual BYTES             GetSlotSize()               const { return m_pHistCacheItemWork->GetItemSize();             }
	virtual bool              IsInHistoryMode()           const { return GetCurrentGeneration() < GetYoungestGeneration(); };

	virtual void              ShutDownHistCache() { m_pHistoryCache->ShutDownHistCache(); }

	virtual ModelData const * CreateAppCommand(GenerationCmd);
    virtual ModelData const * ApproachHistGen (HistGeneration const);
	virtual GenerationCmd     GetGenerationCmd(HistGeneration const);
	virtual ModelData const * GetModelData    (HistGeneration const);
	virtual void              ClearHistory    (HistGeneration const);
	virtual void              ClearAllHistory (); 

	virtual HistoryIterator * CreateHistoryIterator() const;

    virtual HistGeneration   FindGenerationWithProperty(GenerationProperty const &, bool const) const;

	virtual HistoryCache const * GetHistoryCache() const
	{
		return m_pHistoryCache;
	}

private:

    GenCmdList         m_GenCmdList;
    HistoryCache     * m_pHistoryCache;
    HistCacheItem    * m_pHistCacheItemWork;  // The reference item, where history system gets and restores data 

	ModelData  const * save2History();
    void               step2NextGeneration(GenerationCmd);
    void               checkHistoryStructure();
};

