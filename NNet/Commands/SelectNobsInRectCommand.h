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
	SelectNobsInRectCommand
	( 
		MicroMeterRect const & rect,
		bool           const   bAdd2Selection
	)
	  :	m_rect( rect ),
		m_bAdd2Selection( bAdd2Selection )
	{ }

	virtual void Do( NNetModelWriterInterface & nmwi )
	{ 
		UPNobList & list { nmwi.GetUPNobs() };
		SelectionCommand::Do( nmwi );
		if ( ! m_bAdd2Selection )
			list.DeselectAllNobs();
		list.Apply2AllInRect<Nob>(m_rect, [&](Nob& s){ s.Select(true, true); });
	}

private:
	MicroMeterRect const m_rect;
	bool           const m_bAdd2Selection;
};
