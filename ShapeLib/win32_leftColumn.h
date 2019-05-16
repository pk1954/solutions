// win32_leftColumn.cpp
//

#pragma once

#include "win32_identifierShape.h"
#include "win32_infoShape.h"
#include "win32_shape.h"

class LeftColumn : public Shape
{
public:

	virtual void SetTextDisplay( TextDisplay * pTextDisplay )
	{
		Shape::SetTextDisplay( pTextDisplay );
		m_idShape  .SetTextDisplay( pTextDisplay );
		m_infoShape.SetTextDisplay( pTextDisplay );
	}

	virtual PixelRectSize MinimalSize ( EvolutionCore const * const );
	virtual void          Draw        ( EvolutionCore const * const, GridPoint  const, PixelPoint const );
	virtual Shape const * FindShape   ( PixelPoint const, GridPoint const ) const;
	virtual void          PrepareShape( PixelPoint const, PixelRectSize const );

	IdentifierShape const & GetIdentifierShape() const { return m_idShape; }

private:
	IdentifierShape m_idShape;
	InfoShape       m_infoShape;
};
