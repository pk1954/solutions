// EvoModelFactory.h
//
// EvoHistGlue

#pragma once

#include "EvolutionCore.h"
#include "EvoModelDataGlue.h"

class EvoModelFactory: public ModelFactory
{
public:
	virtual EvoModelDataGlue * CreateModelData() const 
	{
		return new EvoModelDataGlue( );                  //ok
	}

	virtual void DestroyModelData( ModelData * pData )
	{
		delete pData;    //ok
	}
};
