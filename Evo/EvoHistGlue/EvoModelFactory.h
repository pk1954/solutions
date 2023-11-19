// EvoModelFactory.h
//
// EvoHistGlue

module;

import EvolutionCore;
import EvoModelDataGlue;
import ModelData;

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
