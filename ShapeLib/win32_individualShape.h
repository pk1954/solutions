// win32_individualShape.h
//

#pragma once

#include "win32_shape.h"
#include "win32_leftColumn.h"
#include "win32_rightColumn.h"

class IndividualShape : public Shape
{
public:
	IndividualShape( TextDisplay & t ) :
		Shape( t ),
		m_leftColumn( t ),
		m_rightColumn( t )
	{}

	virtual void          PrepareShape( PixelPoint const, PixelRectSize const );
	virtual PixelRectSize MinimalSize ( EvolutionCore const * const );
	virtual void          Draw        ( EvolutionCore const * const, GridPoint  const, PixelPoint    const );
	virtual Shape const * FindShape   ( EvolutionCore const * const, PixelPoint const, GridPoint     const ) const;

	LeftColumn  const & GetLeftColumn () const { return m_leftColumn;  }
	RightColumn const & GetRightColumn() const { return m_rightColumn; }

private:
	LeftColumn  m_leftColumn;
	RightColumn m_rightColumn;
};
