// ModelData.ixx
//
// History

export module ModelData;

import GenerationCmd;

export class ModelData
{
public:
	virtual ~ModelData() {};

	// call back functions 

	virtual size_t GetModelSize() const = 0;
	virtual void  CopyFrom(ModelData const* const) = 0;
	virtual void  OnAppCommand(GenerationCmd const) = 0;
	virtual void  Compute() = 0;
};

export class ModelFactory
{
public:
	virtual ModelData* CreateModelData() const = 0;
};
