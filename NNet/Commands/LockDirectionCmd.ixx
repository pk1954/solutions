// LockDirectionCmd.ixx
//
// Commands

module;

#include <vector>

export module NNetCommands:LockDirectionCmd;

import Types;
import :NNetCommand;
import NNetModel;

using std::vector;

export class LockDirectionCmd : public NNetCommand
{
public:
	LockDirectionCmd(vector<IoLine*>&ioLines)
		: m_ioLines(ioLines)
	{
		for (auto& it : m_ioLines)
		{
			m_ioLineState.push_back(it->GetState());
			m_pIoConnector.push_back(it->GetParentNob());
		}
	}

	void Do() final
	{
		for (auto& it : m_ioLines)
			it->LockDirection();
		(m_targetReachedFunc)();
	}

	void Undo() final
	{
		int i = 0;
		for (auto& it : m_ioLines)
		{
			it->SetState(m_ioLineState[i], m_pIoConnector[i]);
			++i;
		}
		m_ioLineState.clear();
		m_pIoConnector.clear();
		(m_targetReachedFunc)();
	}

private:
	vector<IoLine::State> m_ioLineState;
	vector<Nob *>         m_pIoConnector;
	vector<IoLine*>       m_ioLines;
};
