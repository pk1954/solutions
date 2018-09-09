// EvoModelFactory.h
//

#pragma once

#include "EvolutionCore.h"
#include "EvoModelDataGlue.h"

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

	virtual EvoModelDataGlue * CreateModelData() const 
	{
		return new EvoModelDataGlue( m_pEvolutionCore );
	}

private:
    EvolutionCore * const m_pEvolutionCore;
};
