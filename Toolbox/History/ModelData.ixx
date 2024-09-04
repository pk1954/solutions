// ModelData.ixx
//
// History

export module HistoryLib:ModelData;

import std;
import :GenerationCmd;

using std::unique_ptr;

export class ModelData
{
public:
	virtual ~ModelData() {};

	// call back functions 

	virtual size_t GetModelSize() const = 0;
	virtual void   CopyFrom(ModelData const&) = 0;
	virtual void   OnAppCommand(GenerationCmd const) = 0;
	virtual void   Compute() = 0;
};

export class ModelFactory
{
public:
	virtual unique_ptr<ModelData> CreateModelData() const = 0;
};
