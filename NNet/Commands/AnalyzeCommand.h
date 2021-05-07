// AnalyzeLoopsCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"
#include "Analyzer.h"
#include "SelectionCommand.h"

class AnalyzeCommand : public SelectionCommand
{
public:
	AnalyzeCommand(NobStack const stack)
		: m_result(stack)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		SelectionCommand::Do( nmwi );
		if ( ! m_result.IsEmpty() )
		{
			nmwi.GetUPNobs().DeselectAllNobs();
			m_result.Apply2All([&](Nob const &s){ nmwi.SelectNob(s.GetId(), true); }	);
		}
	}

protected:
	NobStack m_result;
};
