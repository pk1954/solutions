// EvoModelDataGlue.h
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoModelDataGlue is ** below ** HISTORY (is called by HISTORY)
//
// EvoHistGlue

module;

import GridDimensions;
#include "ModelData.h"
#include "EvolutionCore.h"
#include "win32_stopwatch.h"

import GridRect;

class EvoModelDataGlue: public ModelData
{
public:
	
    EvoModelDataGlue()
    { 
		m_pEvolutionCore = EvolutionCore::CreateCore();
	}

	~EvoModelDataGlue()
	{
		EvolutionCore::DestroyModel(m_pEvolutionCore);
	}

    EvoModelDataGlue & operator= (EvoModelDataGlue const &);  // noncopyable class 

	EvolutionCore * GetEvolutionCore()
	{
		return m_pEvolutionCore;
	}

	EvolutionCore const * GetEvolutionCoreC() const
	{
		return m_pEvolutionCore;
	}

	virtual BYTES GetModelSize() const
	{
		return m_pEvolutionCore->GetCoreSize() + BYTES(sizeof(EvoModelDataGlue));
	}

	virtual void CopyFrom(ModelData const * const src)
	{
//		stopwatch.Start();
		m_pEvolutionCore->CopyModelData(static_cast< EvoModelDataGlue const * const >(src)->m_pEvolutionCore);
//		stopwatch.Stop(L"Copy model");
	}

    GridPoint FindGridPointFromId(IND_ID const id) const
	{ 
		return m_pEvolutionCore->FindGridPointFromId(id);
	}

	virtual void OnAppCommand(GenerationCmd const);

	virtual void Compute()
	{
		m_pEvolutionCore->Compute();  // compute next generation
	}

	virtual void ResetAll()
	{
		m_pEvolutionCore->ResetAll();
	}

private:
   static Stopwatch stopwatch;

   EvolutionCore * m_pEvolutionCore;
};
