// HistModelData.ixx
//
// History

export module HistModelData;

import ModelData;
import GenerationCmd;

class HistModelData : public ModelData
{
public:
	virtual ~HistModelData( ) {};

	virtual void OnAppCommand( GenerationCmd const )  = 0;
};

class HistModelFactory : public ModelFactory
{
public:
	virtual HistModelFactory * CreateHistModelData( ) const = 0;
};
