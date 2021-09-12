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

	virtual void Do()
	{ 
		SelectionCommand::Do();
		m_pNMWI->GetUPNobs().Apply2AllInRect<Nob>
		(
			m_rect, 
			[&](Nob& s)
			{ 
				if (s.IsPipe())
				{
					Pipe & pipe { static_cast<Pipe &>(s) };
					if (!m_rect.Includes(pipe.GetStartPoint()))
						return;
					if (!m_rect.Includes(pipe.GetEndPoint()))
						return;
				}
				s.Select(true);
			}
		);
	}

private:
	MicroMeterRect const m_rect;
};
