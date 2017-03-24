// EvoModelFactory.h
//

#pragma once

#include "EvolutionCore.h"
#include "EvolutionModelData.h"
#include "EvoModelData.h"

class EvoModelFactory: public ModelFactory
{
public:
	EvoModelFactory( EvolutionCore * const pEvolutionCore )
		: m_pEvolutionCore( pEvolutionCore )
	{ }

	virtual EvoModelData * CreateModelData() const 
	{
		return new EvoModelData( m_pEvolutionCore, EvolutionModelData::CreateModelData( ) );
	}

private:
	EvolutionCore * const m_pEvolutionCore;
};
