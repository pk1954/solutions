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
					Knot const & knot { static_cast<Knot const &>(s) };
					bool const   bIn  { startPointInRect(knot) };
					bool const   bOut { endPointInRect  (knot) };
					if ( ! (bIn||bOut) )
						return;          // knot would be orphan in selection
				}
				s.Select(true);
			}
		);
	}

private:

	MicroMeterRect const m_rect;

	bool startPointInRect(Knot const& knot)
	{
		return knot.Apply2AllInPipesB
		(
			[this](Pipe const& p)
			{
				return m_rect.Includes(p.GetStartPoint());
			}
		);
	}

	bool endPointInRect(Knot const& knot)
	{
		return knot.Apply2AllOutPipesB
		(
			[this](Pipe const& p)
			{
				return m_rect.Includes(p.GetEndPoint());
			}
		);
	}
};
