// LeftColumn.cpp
//
// ShapeLib

module ShapeLib:LeftColumn;

import std;

// Can be displayed, if at least InfoShape has space
// If possible, display also IdShape

PixelRectSize LeftColumn::MinimalSize(EvolutionCore const &core)  
{       
	PixelRectSize const minInfo = m_infoShape.MinimalSize(core);

	return SetMinSize(minInfo);     
}                                     

void LeftColumn::PrepareShape(PixelPoint const ppOffset, PixelRectSize const ppSize)
{
	if (SetShapeRect(ppOffset, ppSize))
	{
		m_infoShape.PrepareShape(GetShapePos(), PixelRectSize(getShapeWidth (), getShapeHeight()));
	}
}

void LeftColumn::Draw
(
	EvolutionCore const &core, 
	GridPoint     const  gp, 
	PixelPoint    const  pntGridpointOffset 
)
{
	if (IsNotEmpty ())
	{
		m_infoShape.Draw(core, gp, pntGridpointOffset);
	}
}

Shape const * LeftColumn::FindShape
(
	PixelPoint const pnt,
	GridPoint  const gp
) const
{
	Shape const * pShapeRes = m_infoShape.FindShape(pnt, gp);
	if (pShapeRes != nullptr)
		return pShapeRes;

	return Shape::FindShape(pnt, gp);
}
