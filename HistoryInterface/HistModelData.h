// HistModelData.h : 
//

#pragma once

#include "ModelData.h"
#include "GenerationCmd.h"

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
