// win32_infoShape.cpp
//

#pragma once

#include <iomanip>     
#include "win32_identifierShape.h"
#include "win32_shape.h"

class EvolutionCore;

class InfoShape : public Shape
{
public:
	virtual void FillBuffer( EvolutionCore const * const, GridPoint const );
};
