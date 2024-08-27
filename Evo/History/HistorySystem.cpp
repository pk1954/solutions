// HistorySystem.cpp
//
// History

module HistoryLib:HistorySystem;

import std;
import Debug;
import SaveCast;
import :HistSlot;
import :HistSlotNr;
import :HistoryCache;
import :HistoryIterator;
import :HistGeneration;
import ObserverInterface;

using std::min;

//
//HistorySystem * HistorySystem::CreateHistorySystem()
//{
//	return new HistorySystem();
//}
//
//size_t HistorySystem::GetSlotWrapperSize()
//{ 
//	return sizeof(HistCacheItem);  
//}
// public member functions

HistorySystem::HistorySystem() :
    m_GenCmdList        (),
    m_pHistoryCache     (nullptr),
    m_pHistCacheItemWork(nullptr)
{ }

ModelData * HistorySystem::StartHistorySystem
(
	HistGeneration     const genMaxNrOfGens,
	long                const lHistEntriesDemanded,
	unsigned long long  const ullMemorySize,
	ModelFactory      * const pModelFactory
)
{
	m_pHistCacheItemWork = new HistCacheItem(pModelFactory);  //ok
	     
	size_t              const slotSize        { GetSlotSize() };
	unsigned long long  const ullMaxNrOfSlots { ullMemorySize / slotSize };    AssertLong(ullMaxNrOfSlots);
	unsigned long long  const ullDemanded     { static_cast<unsigned long long>(lHistEntriesDemanded) };
	unsigned long long  const ullHistEntries  { min(ullDemanded, ullMaxNrOfSlots * 70 / 100) };  // use only 70% of available memory
	HistSlotNr          const nrOfSlots       { Cast2Short(ullHistEntries) }; 

	m_pHistoryCache = new HistoryCache;                    //ok
	m_GenCmdList.Resize(genMaxNrOfGens);
	m_pHistoryCache->InitHistoryCache(nrOfSlots, pModelFactory);
	m_pHistCacheItemWork->SetGenerationCommand(GenerationCmd::ResetCmd(0));
    save2History();
	NotifyAll(false);
	return m_pHistCacheItemWork->GetModelData();
}

void HistorySystem::StopHistorySystem()
{
	ShutDownHistCache();

	delete m_pHistCacheItemWork;  //ok
	delete m_pHistoryCache;       //ok

	m_pHistCacheItemWork = nullptr;
	m_pHistoryCache      = nullptr;

	UnregisterAllObservers();
}

HistorySystem::~HistorySystem()
{
}

// ClearHistory
// Erase all generations starting with genFirst up to youngest generation
// from m_GenerationCmd and from history slots

void HistorySystem::ClearHistory(HistGeneration const genFirst)
{
	for (HistGeneration gen = GetYoungestGeneration(); gen > genFirst; --gen)
	{
		if  (m_GenCmdList.IsCachedGeneration(gen))
			m_pHistoryCache->RemoveHistCacheSlot(m_GenCmdList[gen].GetSlotNr());

		m_GenCmdList.ResetGenerationCmd(gen);
	}
	NotifyAll(false);
}

void HistorySystem::ClearAllHistory() 
{ 
	ApproachHistGen(0);
	Assert(GetCurrentGeneration() == 0);
	ClearHistory(0);
}

bool HistorySystem::AddHistorySlot()
{ 
	bool bRes = m_pHistoryCache->AddCacheSlot();
	NotifyAll(false);
	return bRes;
}

HistoryIterator * HistorySystem::CreateHistoryIterator() const 
{ 
	return new HistoryIterator(m_pHistoryCache);     	 
}

ModelData const * HistorySystem::CreateAppCommand(GenerationCmd const genCmd)
{
	step2NextGeneration(genCmd);
	m_pHistCacheItemWork->SetGenerationCommand(genCmd);
	return save2History();
}

// ApproachHistGen - Get closer to demanded HistGeneration
//                 - If several steps are neccessary, function returns after one displayed generation
//                   to allow user interaction
//                 - But actual history generation is altered by at least 1

ModelData const * HistorySystem::ApproachHistGen(HistGeneration const genDemanded)
{
    HistGeneration genActual = m_pHistCacheItemWork->GetHistGenCounter();

    Assert(genDemanded != genActual);
	Assert(genDemanded <= m_GenCmdList.GetMaxGeneration());
	Assert(m_GenCmdList[0].IsCachedGeneration());      // at least initial generation is cached

    HistGeneration genCached   = genDemanded;  // search backwards starting with genDemanded
    bool           bMicrosteps = true;
        
    while (m_GenCmdList[genCached].IsNotCachedGeneration())
        --genCached;

    // now we have found a cached generation  

    if (
          ((genCached <= genActual) && (genActual < genDemanded)) || // cached generation is not better than actual generation
          ((genActual == genDemanded - 1) && bMicrosteps)
      )
    {
		step2NextGeneration(m_GenCmdList[genActual + 1]);   // compute forwards
    }
    else  // get cached generation
    {
		HistSlotNr const slotNr = m_GenCmdList[genCached].GetSlotNr();
		m_pHistoryCache->CopyFromCacheSlot(slotNr, m_pHistCacheItemWork);
    }

	NotifyAll(false);

	return nullptr;
}

GenerationCmd HistorySystem::GetGenerationCmd(HistGeneration const gen)
{
	GenerationCmd cmd = m_GenCmdList[gen];

	if (cmd.IsCachedGeneration())
	{
		HistSlotNr    const   sSlotNr = cmd.GetSlotNr();
		HistCacheItem const * pItem   = m_pHistoryCache->GetHistCacheItemC(sSlotNr);
		cmd = pItem->GetGenCmd();
	}

	return cmd;
}

ModelData const * HistorySystem::GetModelData(HistGeneration const gen)
{
	ModelData     const * pModelData { nullptr };
	GenerationCmd const   cmd { m_GenCmdList[gen] };

	if (cmd.IsCachedGeneration())
		pModelData = m_pHistoryCache->GetHistCacheItemC(cmd.GetSlotNr())->GetModelDataC();

	return pModelData;
}

// private member functions

// save2History - Save Generation data (rule how to get to next generation), current GridModel, etc. to new slot
//                Return pointer to this slot to be used for reading

ModelData const * HistorySystem::save2History()
{
    HistSlotNr    const   slotNr          = m_pHistoryCache->GetFreeCacheSlot();
    HistCacheItem const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC(slotNr);
    GenerationCmd const   genCmdFromCache = pHistCacheItem->GetGenCmd();

    if (genCmdFromCache.IsDefined())       // Hist slot was in use before. Save GenCommand
    {
        HistGeneration const genCached = pHistCacheItem->GetHistGenCounter();
        Assert(m_GenCmdList[genCached].IsCachedGeneration());
        Assert(m_GenCmdList[genCached].GetSlotNr() == slotNr);
        m_GenCmdList.SetGenerationCmd(genCached, genCmdFromCache);
        m_pHistoryCache->ResetHistCacheSlot(slotNr);
    }

//    CHECK_HISTORY_STRUCTURE;
    ModelData const * pModelData = m_pHistoryCache->Save2CacheSlot(slotNr, m_pHistCacheItemWork);
    m_GenCmdList.SetCachedGeneration(m_pHistCacheItemWork->GetHistGenCounter(), slotNr);
//    CHECK_HISTORY_STRUCTURE;

	return pModelData;
};

// step2NextGeneration - if cached generation: get GenerationCmd from cache
//                     - apply command and increment history generation counter
// throws exception if maximum number of generations reached

void HistorySystem::step2NextGeneration(GenerationCmd genCmd)
{
	ModelData * pModelData = m_pHistCacheItemWork->GetModelData();
	
	if (genCmd.IsCachedGeneration()) // can happen only in history mode
    {
        Assert(IsInHistoryMode());
        HistSlotNr    const   slotNr         = genCmd.GetSlotNr();
        HistCacheItem const * pHistCacheItem = m_pHistoryCache->GetHistCacheItemC(slotNr);
        genCmd = pHistCacheItem->GetGenCmd();
    }

    if (m_pHistCacheItemWork->GetHistGenCounter() >= m_GenCmdList.GetMaxGeneration())
		throw HistoryBufferException();

	if (genCmd.GetCommand() == GenerationCmd::Id::NEXT_GEN)
	{
		pModelData->Compute();  // compute next generation
	}
	else
	{
		pModelData->OnAppCommand(genCmd);    // Apply application defined operation to step to next generation
	}

	m_pHistCacheItemWork->IncHistGenCounter();
}

HistGeneration HistorySystem::FindGenerationWithProperty
(
	GenerationProperty const & property,
	bool               const   bReverse   // if true, search from youngest to oldest generation
) const
{
    HistoryIterator histIter(m_pHistoryCache);

    for (
			HistSlotNr slotNr = bReverse ? histIter.Set2Youngest() : histIter.Set2Oldest(); 
			slotNr.IsNotNull(); 
			slotNr = bReverse ? histIter.Set2Senior() : histIter.Set2Junior() 
		)
    {
        if ((property)(m_pHistoryCache->GetHistCacheItemC(slotNr)->GetModelDataC()))
        {
            return m_pHistoryCache->GetGridGen(slotNr);
        }
    }

    return HistGeneration();   // NULL_VAL: no generation found with property
}

void HistorySystem::checkHistoryStructure()  // used only in debug mode
{
     // check generation number of all generations in history cache

//	wcout << L"**** checkHistoryStructure" << endl;
//	wcout << L"m_genCmdList" << endl;

    for (HistGeneration gen = 0; gen <= m_GenCmdList.GetMaxGeneration(); ++gen)
    {
        GenerationCmd generationCmd = m_GenCmdList[gen];
//		if (generationCmd.IsDefined())
//			wcout << gen 
//			      << L" (" << generationCmd.GetCommand() 
//				  << L"," << generationCmd.GetParam()
//			      << L")";
        if (generationCmd.IsCachedGeneration())
        {
            HistSlotNr      const   slotNrFromList  = generationCmd.GetSlotNr();
            HistCacheItem   const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC(slotNrFromList);
            HistGeneration const   genNrFromCache  = pHistCacheItem->GetHistGenCounter();
	        GenerationCmd   const   genCmdFromCache = pHistCacheItem->GetGenCmd();
            Assert(genNrFromCache == gen);
//			wcout << L" cache: slot "
//				  << sSlotNrFromList
//	              << L" (" << genCmdFromCache.GetCommand() << L"," << genCmdFromCache.GetParam() << L") "
//				  << genNrFromCache << endl;
        }
        else if (generationCmd.IsUndefined())
            break;
//		else
//			wcout << endl;
    }

    // check slot numbers and according generation commands

//	wcout << L"slots" << L"(" << m_pHistoryCache->GetNrOfHistCacheSlots() << L")" << endl;

    for (HistSlotNr slotNr = HistSlotNr(0); slotNr < m_pHistoryCache->GetNrOfHistCacheSlots(); ++slotNr)
    {
        HistCacheItem   const * pHistCacheItem  = m_pHistoryCache->GetHistCacheItemC(slotNr);
        HistGeneration const   genNrFromCache  = pHistCacheItem->GetHistGenCounter();
        GenerationCmd   const   genCmdFromCache = pHistCacheItem->GetGenCmd();
        if (genCmdFromCache.IsDefined())
        {
            GenerationCmd const generationCmd   = m_GenCmdList[genNrFromCache];
            HistSlotNr    const slotNrFromList = generationCmd.GetSlotNr();
//			wcout << L"slot " << sSlotNr 
//				  << L" (" << genCmdFromCache.GetCommand() << L"," << genCmdFromCache.GetParam() << L") "
//				  << L"gen " << genNrFromCache
//				  << L" slotNrFromList= " << slotNrFromList 
//				  << endl;
            Assert(generationCmd.IsCachedGeneration());
            Assert(slotNrFromList == slotNr);
        }
    }
}
