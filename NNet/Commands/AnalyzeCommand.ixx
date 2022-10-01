// AnalyzeCommand.ixx
//
// Commands

export module AnalyzeCommand;

import SelectionCommand;
import NNetModel;

export class AnalyzeCommand : public SelectionCommand
{
public:
	explicit AnalyzeCommand(NobStack const & stack)
		: m_result(stack)
	{}

	void Do() final 
	{ 
		SelectionCommand::Do();
		if (!m_result.empty())
		{
			m_pNMWI->DeselectAllNobs();
			for (auto it : m_result) { m_pNMWI->SelectNob(it->GetId(), true); }
		}
	}

private:
	NobStack m_result;
};
