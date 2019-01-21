// win32_infoShape.cpp
//

#pragma once

#include <iomanip>     
#include "plannedActivity.h"
#include "win32_identifierShape.h"
#include "win32_shape.h"

class InfoShape : public Shape
{
public:

	InfoShape( TextDisplay & t ) :
		Shape( t )
	{}

	virtual void FillBuffer( GridPoint const );
};
