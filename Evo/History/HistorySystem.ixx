// HistorySystem.ixx
//
// History

module;

#include <exception>

export module HistorySystem;

import Observable;
import GenerationCmd;
import HistGeneration;
import HistSlotNr;
import ModelData;
import HistoryIterator;
import HistoryCache;
import ModelData;

export class GenerationProperty  // used when searching for generation with certain property
{
public:
	virtual bool operator() (ModelData const*) const = 0;
};

export class HistoryBufferException : public std::exception { };

export class HistorySystem : public Observable
{
public:
	static HistorySystem* CreateHistorySystem();

	virtual ~HistorySystem() { };

	virtual ModelData* StartHistorySystem
	(
		HistGeneration    const,
		long               const,
		unsigned long long const,
		ModelFactory* const
	) = 0;

	virtual void StopHistorySystem() = 0;

	virtual size_t            GetSlotSize()               const = 0;
	virtual HistSlotNr       GetNrOfUsedHistCacheSlots() const = 0;
	virtual HistSlotNr       GetNrOfHistCacheSlots()     const = 0;
	virtual HistGeneration   GetNrOfGenerations()        const = 0;
	virtual HistGeneration   GetYoungestGeneration()     const = 0;
	virtual HistGeneration   GetCurrentGeneration()      const = 0;
	virtual bool             IsInHistoryMode()           const = 0;
	virtual HistoryIterator* CreateHistoryIterator()     const = 0;
	virtual bool             AddHistorySlot() = 0;

	virtual ModelData const* CreateAppCommand(GenerationCmd  const) = 0;
	virtual ModelData const* ApproachHistGen (HistGeneration const) = 0;
	virtual GenerationCmd    GetGenerationCmd(HistGeneration const) = 0;
	virtual void             ClearHistory    (HistGeneration const) = 0;
	virtual void             ClearAllHistory() = 0;

	virtual HistoryCache const* GetHistoryCache()            const = 0;
	virtual ModelData    const* GetModelData(HistGeneration const) = 0;
	virtual void                ShutDownHistCache()                = 0;

	virtual HistGeneration FindGenerationWithProperty(GenerationProperty const&, bool const) const = 0;

	static  size_t GetSlotWrapperSize();
};
