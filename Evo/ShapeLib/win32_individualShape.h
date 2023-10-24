// win32_individualShape.h
//

module;

#include "win32_leftColumn.h"
#include "win32_rightColumn.h"

import Shape;

class IndividualShape : public Shape
{
public:
	virtual void SetTextDisplay(TextDisplay * pTextDisplay)
	{
		Shape::SetTextDisplay(pTextDisplay);
		m_leftColumn .SetTextDisplay(pTextDisplay);
		m_rightColumn.SetTextDisplay(pTextDisplay);
	}

	virtual void          PrepareShape (PixelPoint const, PixelRectSize const);
	virtual PixelRectSize MinimalSize  (EvolutionCore const * const);
	virtual void          Draw         (EvolutionCore const * const, GridPoint  const, PixelPoint    const);
	virtual Shape const * FindShape    (EvolutionCore const * const, PixelPoint const, GridPoint     const) const;

	LeftColumn  const & GetLeftColumn () const { return m_leftColumn;  }
	RightColumn const & GetRightColumn() const { return m_rightColumn; }

private:
	LeftColumn  m_leftColumn;
	RightColumn m_rightColumn;
};
