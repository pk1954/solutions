// EvoModelFactory.ixx
//
// EvoHistGlue

export module EvoHistGlueLib:EvoModelFactory;

import EvoCoreLib;
import HistoryLib;
import :EvoModelDataGlue;

class EvoModelFactory: public ModelFactory
{
public:
	virtual EvoModelDataGlue * CreateModelData() const 
	{
		return new EvoModelDataGlue();                  //ok
	}

	virtual void DestroyModelData(ModelData * pData)
	{
		delete pData;    //ok
	}
};
