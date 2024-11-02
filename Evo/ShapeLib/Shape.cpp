// Shape.cpp
//
// ShapeLib

module ShapeLib:Shape;

void Shape::Draw
(
	EvolutionCore const &core, 
	GridPoint     const  gp, 
	PixelPoint    const  ppGridPointOffset 
)
{
	if (IsNotEmpty())
	{
		m_pTextDisplay->Clear();
		FillBuffer(&core, gp);
		m_pTextDisplay->DisplayText(m_rect + ppGridPointOffset);
	}
}
