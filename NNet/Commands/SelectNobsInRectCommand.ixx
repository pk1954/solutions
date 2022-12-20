// SelectNobsInRectCommand.ixx
//
// Commands

module;

#include <functional>

export module SelectNobsInRectCommand;

import Types;
import SelectionCommand;
import NNetModel;

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
					Knot const& knot { static_cast<Knot const&>(s) };
					bool const  bIn  { m_rect.Includes(knot.GetIncoming()->GetStartPoint()) };
					bool const  bOut { m_rect.Includes(knot.GetOutgoing()->GetEndPoint()) };
					if (!(bIn || bOut))
						return;          // knot would be orphan in selection
				}
				if (s.IsFork())
				{
					Fork const& fork  { static_cast<Fork const&>(s) };
					bool const  bIn   { m_rect.Includes(fork.GetIncoming      ()->GetStartPoint()) };
					bool const  bOut1 { m_rect.Includes(fork.GetFirstOutgoing ()->GetEndPoint()) };
					bool const  bOut2 { m_rect.Includes(fork.GetSecondOutgoing()->GetEndPoint()) };
					if (!(bIn || bOut1 || bOut2))
						return;          // fork would be orphan in selection
				}
				s.Select(true);
			}
		);
	}

private:

	MicroMeterRect const m_rect;
};
