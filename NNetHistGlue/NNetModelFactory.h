// NNetModelFactory.h
//
// NNetHistGlue

#pragma once

#include "NNetModelDataGlue.h"

class NNetModelFactory: public ModelFactory
{
public:
	virtual NNetModelDataGlue * CreateModelData() const 
	{
		return new NNetModelDataGlue( );           
	}

	virtual void DestroyModelData( ModelData * pData )
	{
		delete pData;
	}
};
