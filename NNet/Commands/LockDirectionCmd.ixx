// LockDirectionCmd.ixx
//
// Commands

module;

#include <vector>

export module LockDirectionCmd;

import Types;
import NNetCommand;
import NNetModel;

using std::vector;

export class LockDirectionCmd : public NNetCommand
{
public:
	LockDirectionCmd(vector<IoLine*>&ioLines)
		: m_ioLines(ioLines)
	{}

	void Do() final
	{
		for (auto& it : m_ioLines)
			m_bLocked.push_back(it->LockDirection());
		(m_targetReachedFunc)();
	}

	void Undo() final
	{
		int i = 0;
		for (auto& it : m_ioLines)
		{
			if (m_bLocked[i])
				it->LockDirection();
			else 
				it->UnlockDirection();
			++i;
		}
		m_bLocked.clear();
		(m_targetReachedFunc)();
	}

private:
	vector<bool>    m_bLocked;
	vector<IoLine*> m_ioLines;
};
