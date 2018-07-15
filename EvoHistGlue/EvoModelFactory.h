// EvoModelFactory.h
//

#pragma once

#include "EvolutionModelData.h"
#include "EvoModelData.h"

class WorkThread;

class EvoModelFactory: public ModelFactory
{
public:
	EvoModelFactory
	( 
        EvolutionCore * const pCore
	) : 
		m_pEvolutionCore( pCore )
	{ }

	virtual EvoModelData * CreateModelData() const 
	{
		return new EvoModelData
		( 
			EvolutionModelData::CreateModelData( ), 
			m_pEvolutionCore
		);
	}

private:
    EvolutionCore * const m_pEvolutionCore;
};
