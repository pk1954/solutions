// EvoModelFactory.h
//

#pragma once

#include "EvolutionCore.h"
#include "EvoModelDataGlue.h"

class EvoModelFactory: public ModelFactory
{
public:
	virtual EvoModelDataGlue * CreateModelData() const 
	{
		return new EvoModelDataGlue( EvolutionCore::CreateCore( ) );
	}
};
