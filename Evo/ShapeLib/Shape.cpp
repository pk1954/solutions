// Shape.cpp
//
// ShapeLib

module ShapeLib:Shape;

void Shape::Draw
(
	EvolutionCore const * const pCore, 
	GridPoint             const gp, 
	PixelPoint            const ppGridPointOffset 
)
{
	if (IsNotEmpty())
	{
		m_pTextDisplay->Clear();
		FillBuffer(pCore, gp);
		m_pTextDisplay->DisplayText(m_rect + ppGridPointOffset);
	}
}
