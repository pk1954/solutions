// GridPointShape.ixx
//
// ShapeLib

export module ShapeLib:GridPointShape;

import D3D_driver;
import EvoCoreLib;
import TextDisplay;
import :Shape;
import :CoordShape;
import :IndividualShape;

export class GridPointShape
{
public:
	GridPointShape()
		: m_pTextDisplay(nullptr)
	{ }

	void Start(TextDisplay* pTextDisplay)
	{
		m_pTextDisplay = pTextDisplay;
		m_shape.SetTextDisplay(pTextDisplay);
		m_coordShape.SetTextDisplay(pTextDisplay);
		m_indivShape.SetTextDisplay(pTextDisplay);
	}

	void          Draw(EvolutionCore const* const, GridPoint const);
	Shape const* FindShape(EvolutionCore const* const, GridPoint const, PixelPoint const) const;
	void          RefreshLayout(EvolutionCore const* const);
	PIXEL         GetIndShapeSize();

	IndividualShape const& GetIndividualShape() const
	{
		return m_indivShape;
	}

private:
	TextDisplay* m_pTextDisplay;
	Shape           m_shape;
	CoordShape      m_coordShape;
	IndividualShape m_indivShape;
};
