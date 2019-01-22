// win32_identifierShape.h
//

#pragma once

#include <iomanip>     
#include "IndividualId.h"
#include "win32_shape.h"

class IdentifierShape : public Shape
{
public:
	IdentifierShape( TextDisplay & t ) :
		Shape( t )
	{}

	virtual void Draw( GridPoint const gp, PixelPoint const ppGridPointOffset )
	{
		Shape::Draw( gp, ppGridPointOffset ); 
	}

	virtual void FillBuffer( GridPoint const gp )
	{
		m_textDisplay.Buffer() << L"ID: " << setw( 5 ) << m_textDisplay.Core().GetId( gp );
	}
};
