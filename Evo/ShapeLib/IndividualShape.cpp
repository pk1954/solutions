// win32_individualShape.cpp
//
// ShapeLib

module ShapeLib:IndividualShape;

PixelRectSize IndividualShape::MinimalSize(EvolutionCore const &core)  
{       
	PixelRectSize minLeft  { m_leftColumn .MinimalSize(core) };
	PixelRectSize minRight { m_rightColumn.MinimalSize(core) };

	return SetMinSize(minLeft);
}                                     

void IndividualShape::PrepareShape(PixelPoint const ppOffset, PixelRectSize const ppSize)
{
	if (SetShapeRect(ppOffset, ppSize))
	{
		PIXEL pixWidth  { getShapeWidth()  };
		PIXEL pixHeight { getShapeHeight() };

		PixelPoint pixPosSubShape { GetShapePos() };
		PIXEL      pixHalfWidth   { pixWidth / 2 };

		if (pixHalfWidth >= m_leftColumn.GetMinWidth())  // if left column fits in half of available space
		{
			PixelRectSize pixSizeSubShape { PixelRectSize(pixHalfWidth, pixHeight) };
			m_leftColumn .PrepareShape(pixPosSubShape, pixSizeSubShape);
			pixPosSubShape += PixelPoint(pixHalfWidth, 0_PIXEL);
			m_rightColumn.PrepareShape(pixPosSubShape, pixSizeSubShape);
		}
		else
		{
			m_leftColumn .PrepareShape(pixPosSubShape, PixelRectSize(pixWidth, pixHeight));
			m_rightColumn.SetShapeEmpty();
		}
	}
}

void IndividualShape::Draw(EvolutionCore const &core, GridPoint const gp, PixelPoint const ppGridPointOffset)
{
	if (IsNotEmpty ())
	{
		m_leftColumn. Draw(core, gp, ppGridPointOffset);
		m_rightColumn.Draw(core, gp, ppGridPointOffset);
	}
}

Shape const * IndividualShape::FindShape
(
	EvolutionCore const &core, 
	PixelPoint    const  pnt, 
	GridPoint     const  gp
) const
{
	Shape const * pShapeRes = m_leftColumn.FindShape(pnt, gp);
	if (pShapeRes != nullptr)
		return pShapeRes;

	if (core.GetStrategyId(gp) == Strategy::Id::tit4tat)
	{
		pShapeRes = m_rightColumn.FindShape(core, pnt, gp);
		if (pShapeRes != nullptr)
			return pShapeRes;
	}

	return Shape::FindShape(pnt, gp);
}
