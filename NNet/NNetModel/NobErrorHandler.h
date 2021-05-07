// NNetModel.h 
//
// NobErrorHandler.h

#pragma once

#include "NobId.h"

class NobErrorHandler
{
public:
	virtual void operator()( NobId const ) = 0;
};
