// RightColumn.ixx
//
// ShapeLib

export module ShapeLib:RightColumn;

import std;
import EvoCoreLib;
import :Shape;
import :MemorySlot;
import :TextDisplay;

class RightColumn : public Shape
{
public:
	RightColumn();

	virtual void          SetTextDisplay(TextDisplay *);
	virtual void          PrepareShape(PixelPoint const, PixelRectSize const);
	virtual PixelRectSize MinimalSize (EvolutionCore const&);
	virtual void          Draw        (EvolutionCore const&, GridPoint const, PixelPoint const );
	virtual void          FillBuffer  (EvolutionCore const&, GridPoint const);

	virtual Shape const * FindShape(EvolutionCore const&, PixelPoint const, GridPoint const) const;

private:
    std::array< MemorySlot *, IMEMSIZE_MAX > m_aMemorySlot; 
};
