// SelectNobsInRectCommand.ixx
//
// Commands

module;

#include "Pipe.h"
#include "Knot.h"
#include "NNetModelWriterInterface.h"

export module SelectNobsInRectCommand;

import MoreTypes;
import SelectionCommand;

export class SelectNobsInRectCommand : public SelectionCommand
{
public:
	explicit SelectNobsInRectCommand(MicroMeterRect const & rect)
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
					Pipe const & pipe { static_cast<Pipe const &>(s) };
					if (!m_rect.Includes(pipe.GetStartPoint()))
						return;
					if (!m_rect.Includes(pipe.GetEndPoint()))
						return;
				}
				if (s.IsKnot())
				{
					Knot const & knot { static_cast<Knot const &>(s) };
					bool const bIn  { knot.Apply2AllInPipesB ([&](Pipe const & p){ return m_rect.Includes(p.GetStartPoint()); }) };
					bool const bOut { knot.Apply2AllOutPipesB([&](Pipe const & p){ return m_rect.Includes(p.GetEndPoint  ()); }) };
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
