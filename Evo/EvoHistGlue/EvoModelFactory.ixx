// EvoModelFactory.ixx
//
// EvoHistGlue

export module EvoHistGlueLib:EvoModelFactory;

import std;
import EvoCoreLib;
import HistoryLib;
import :EvoModelDataGlue;

using std::unique_ptr;
using std::make_unique;

class EvoModelFactory: public ModelFactory
{
public:
	virtual unique_ptr<EvoModelDataGlue>CreateModelData() const 
	{
		return make_unique<EvoModelDataGlue>();
	}

	virtual void DestroyModelData(ModelData * pData)
	{
		delete pData;    //ok
	}
};
