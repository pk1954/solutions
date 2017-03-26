// EvoModelFactory.h
//

#pragma once

#include "EvolutionCore.h"
#include "EvolutionModelData.h"
#include "EvoModelData.h"

class WorkThread;

class EvoModelFactory: public ModelFactory
{
public:
	EvoModelFactory( EvolutionCore * const pEvolutionCore, WorkThread * const pWorkThread ) : 
		m_pEvolutionCore( pEvolutionCore ),
		m_pWorkThread( pWorkThread )
	{ }

	virtual EvoModelData * CreateModelData() const 
	{
		return new EvoModelData( m_pEvolutionCore, EvolutionModelData::CreateModelData( ), m_pWorkThread );
	}

private:
	EvolutionCore * const m_pEvolutionCore;
	WorkThread    * const m_pWorkThread;
};
