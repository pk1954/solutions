// ModelData.h : 
//
// History

module;

#include "GenerationCmd.h"

export module ModelData;

export class ModelData 
{
public:
    virtual ~ModelData() {};

	// call back functions 

	virtual BYTES GetModelSize() const                   = 0;   
	virtual void  CopyFrom    (ModelData const * const) = 0;
	virtual void  OnAppCommand(GenerationCmd const)     = 0;
	virtual void  Compute()                              = 0;
};

class ModelFactory
{
public:
	virtual ModelData * CreateModelData() const = 0;
};
