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

	void Do() final
	{ 
		SelectionCommand::Do();
		m_pNMWI->DeselectAllNobs();
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
				if (s.IsKnot())
				{
					Knot & knot { static_cast<Knot &>(s) };
					bool bIn  { knot.Apply2AllInPipesB ([&](Pipe const & p){ return m_rect.Includes(p.GetStartPoint()); }) };
					bool bOut { knot.Apply2AllOutPipesB([&](Pipe const & p){ return m_rect.Includes(p.GetEndPoint  ()); }) };
					if ( ! (bIn||bOut) )
						return;          // knot would be orphan in selection
				}
				s.Select(true);
			}
		);
	}

private:
	MicroMeterRect const m_rect;
};
