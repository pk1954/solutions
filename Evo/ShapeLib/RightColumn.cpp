// RightColumn.cpp
//
// ShapeLib

module ShapeLib:RightColumn;

import std;
import :MemorySlot;

using std::wostringstream;

RightColumn::RightColumn() :
	Shape(),
	m_aMemorySlot()
{
	for	(MEM_INDEX mem = MEM_INDEX(0); mem < MEM_INDEX(IMEMSIZE_MAX); ++mem)
	{
		m_aMemorySlot[mem.GetValue()] = new MemorySlot(mem);
	}
}

void RightColumn::SetTextDisplay(TextDisplay * pTextDisplay)
{
	Shape::SetTextDisplay(pTextDisplay);
	for	(auto & pSlot : m_aMemorySlot)
	{
		pSlot->SetTextDisplay(pTextDisplay);
	}
}

PixelRectSize RightColumn::MinimalSize(EvolutionCore const &core)  
{       
	PixelRectSize minSlotSize { m_aMemorySlot[0]->MinimalSize(core) };
	return SetMinSize(minSlotSize.GetX(), minSlotSize.GetY() * IMEMSIZE_MAX	);     
}                                     

void RightColumn::PrepareShape(PixelPoint const ppOffset, PixelRectSize const ppSize)
{
	if (SetShapeRect(ppOffset, ppSize))
	{
		PIXEL         const pixSlotHeight { getShapeHeight() / IMEMSIZE_MAX };
		PixelRectSize const slotSize      { getShapeWidth(), pixSlotHeight };

		PixelPoint posShape = GetShapePos();
		for	(auto & pSlot : m_aMemorySlot)
		{
			posShape += PixelPoint(0_PIXEL, pixSlotHeight);
			pSlot->PrepareShape(posShape, slotSize);
		}
	}
}

void RightColumn::FillBuffer(EvolutionCore const &core, GridPoint const gp)
{
	wostringstream &buffer { m_pTextDisplay->Buffer() };

	MEM_INDEX const memSize { core.GetMemSize(gp) };
	MEM_INDEX const memUsed { core.GetMemUsed(gp) }; 
        
	buffer << L"Memory " << memUsed.GetValue() << L"/" << memSize.GetValue();
}

Shape const * RightColumn::FindShape
(
	EvolutionCore const &core, 
	PixelPoint    const  pnt,             
	GridPoint     const  gp
) const
{
	MEM_INDEX const memUsed { core.GetMemUsed(gp) };
	for	(auto & pSlot : m_aMemorySlot)
	{
		if (pSlot->GetMemIndex() == memUsed)
			break;
		if (pSlot->FindShape(pnt, gp))
			return pSlot;
	}

	return Shape::FindShape(pnt, gp);
}

void RightColumn::Draw
(
	EvolutionCore const &core, 
	GridPoint     const  gp, 
	PixelPoint    const  pntGridpointOffset 
)
{
	if (IsNotEmpty ())
	{
		Shape::Draw(core, gp, pntGridpointOffset);

		MEM_INDEX const memUsed { core.GetMemUsed(gp) };
		for	(auto & pSlot : m_aMemorySlot)
		{
			if (pSlot->GetMemIndex() == memUsed)
				break;
			pSlot->Draw(core, gp, pntGridpointOffset);
		}
	}
}
