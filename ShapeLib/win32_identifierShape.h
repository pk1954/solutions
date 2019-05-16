// win32_identifierShape.h
//

#pragma once

#include <iomanip>     
#include "EvolutionTypes.h"
#include "win32_shape.h"

class IdentifierShape : public Shape
{
public:
	virtual void Draw
	( 
		EvolutionCore const * const pCore, 
		GridPoint             const gp, 
		PixelPoint            const pntGridpointOffset 
	)
	{
		Shape::Draw( pCore, gp, pntGridpointOffset ); 
	}

	virtual void FillBuffer( EvolutionCore const * const pCore, GridPoint const gp )
	{
		m_pTextDisplay->Buffer() << L"ID: " << pCore->GetId( gp );
	}
};
