// LeftColumn.ixx
//
// ShapeLib

export module ShapeLib:LeftColumn;

import :Shape;
import :InfoShape;
import :TextDisplay;

class LeftColumn : public Shape
{
public:

	virtual void SetTextDisplay(TextDisplay * pTextDisplay)
	{
		Shape::SetTextDisplay(pTextDisplay);
		m_infoShape.SetTextDisplay(pTextDisplay);
	}

	virtual PixelRectSize MinimalSize (EvolutionCore const&);
	virtual void          Draw        (EvolutionCore const&, GridPoint  const, PixelPoint const);
	virtual Shape const * FindShape   (PixelPoint const, GridPoint const) const;
	virtual void          PrepareShape(PixelPoint const, PixelRectSize const);

private:
	InfoShape m_infoShape;
};
