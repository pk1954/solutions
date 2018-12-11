// win32_individualShape.h
//

#pragma once

#include "win32_shape.h"
#include "win32_leftColumn.h"
#include "win32_rightColumn.h"

class IndividualShape : public Shape
{
public:

	IndividualShape
	( 
		Shape * const pParent,
		TextDisplay & textDisplay 
	) :
		Shape     ( pParent, textDisplay ),
		m_leftColumn ( this, textDisplay ),
		m_rightColumn( this, textDisplay )
	{ }

	virtual void PrepareShape( GridPoint const );
	virtual void FillBuffer  ( GridPoint const gp ) { };  // all text in subshapes
	virtual void Draw        ( GridPoint const );

	virtual Shape const * FindShape( PixelPoint const, GridPoint const ) const;

	LeftColumn const & GetLeftColumn() const 
	{
		return m_leftColumn;
	}

private:
	LeftColumn  m_leftColumn;
	RightColumn m_rightColumn;
};
