// EvoModelDataGlue.ixx
//
// Glue code for adapting EVOLUTION to HISTORY system
//
// EvoModelDataGlue is ** below ** HISTORY (is called by HISTORY)
//
// EvoHistGlue

export module EvoHistGlueLib:EvoModelDataGlue;

import EvoCoreLib;
import HistoryLib;

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

	virtual size_t GetModelSize() const
	{
		return m_pEvolutionCore->GetCoreSize() + sizeof(EvoModelDataGlue);
	}

	virtual void CopyFrom(ModelData const * const src)
	{
		m_pEvolutionCore->CopyModelData(static_cast<EvoModelDataGlue const * const>(src)->m_pEvolutionCore);
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

   EvolutionCore * m_pEvolutionCore;
};
