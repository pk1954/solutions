// SelectNobsInRectCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "NNetModelWriterInterface.h"
#include "SelectionCommand.h"

class SelectNobsInRectCommand : public SelectionCommand
{
public:
	SelectNobsInRectCommand(MicroMeterRect const & rect)
	  :	m_rect(rect)
	{ }

	virtual void Do(NNetModelWriterInterface & nmwi)
	{ 
		SelectionCommand::Do(nmwi);
		nmwi.GetUPNobs().Apply2AllInRect<Nob>(m_rect, [&](Nob& s){ s.Select(true); });
	}

private:
	MicroMeterRect const m_rect;
};
