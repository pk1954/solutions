// win32_leftColumn.cpp
//

#pragma once

#include "win32_identifierShape.h"
#include "win32_infoShape.h"
#include "win32_shape.h"

class LeftColumn : public Shape
{
public:
	LeftColumn
	( 
		Shape * const pParent,
		TextDisplay & textDisplay 
	) :
		Shape( pParent, textDisplay ),
		m_idShape  ( this, textDisplay ),
		m_infoShape( this, textDisplay )
	{ }

	virtual void PrepareShape( GridPoint const );
	virtual void Draw        ( GridPoint const );
	virtual void FillBuffer  ( GridPoint const ) { };  // all text in subshapes

	virtual Shape const * FindShape( PixelPoint const, GridPoint const ) const;

	IdentifierShape const & GetIdentifierShape() const 
	{
		return m_idShape;
	}

private:
	IdentifierShape m_idShape;
	InfoShape       m_infoShape;
};
