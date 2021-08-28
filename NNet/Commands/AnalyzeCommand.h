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

	virtual void Do() 
	{ 
		SelectionCommand::Do();
		if (!m_result.empty())
		{
			m_pNMWI->DeselectAllNobs();
			for (auto it : m_result) { m_pNMWI->SelectNob(it->GetId(), true); }
		}
	}

protected:
	NobStack m_result;
};
