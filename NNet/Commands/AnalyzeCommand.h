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

	virtual void Do(NNetModelWriterInterface & nmwi) 
	{ 
		SelectionCommand::Do(nmwi);
		if (!m_result.empty())
		{
			nmwi.DeselectAllNobs();
			for (auto it : m_result) { nmwi.SelectNob(it->GetId(), true); }
		}
	}

protected:
	NobStack m_result;
};
