// ModelData.h : 
//

#pragma once

#include "GenerationCmd.h"

class ModelData 
{
public:
    virtual ~ModelData( ) {};

	// call back functions 

	virtual void CopyModelData( ModelData const * const )     = 0;   
    virtual void OnAppCommand( tGenCmd const, int16_t const ) = 0;
};

class ModelFactory
{
public:
	virtual ModelData * CreateModelData( ) const = 0;
};
