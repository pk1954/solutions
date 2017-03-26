// EvoModelFactory.h
//

#pragma once

#include "EvolutionModelData.h"
#include "EvoModelData.h"

class WorkThread;

class EvoModelFactory: public ModelFactory
{
public:
	EvoModelFactory( WorkThread * const pWorkThread ) : 
		m_pWorkThread( pWorkThread )
	{ }

	virtual EvoModelData * CreateModelData() const 
	{
		return new EvoModelData( EvolutionModelData::CreateModelData( ), m_pWorkThread );
	}

private:
	WorkThread * const m_pWorkThread;
};
