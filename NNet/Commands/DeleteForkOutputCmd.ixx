// DeleteForkOutputCmd.ixx
//
// Commands
//
// handles only the two output Pipes of a Fork
// not the input Pipe

module;

#include <memory>

export module DeleteForkOutputCmd;

import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class DeleteForkOutputCmd : public NNetCommand
{
public:

	explicit DeleteForkOutputCmd(NobId const id)
		: m_id(id)
	{
		Fork* pFork { m_pNMWI->GetNobPtr<Fork*>(m_id) };
		m_upInputLine1 = make_unique<InputLine>(*pFork);
		m_upInputLine2 = make_unique<InputLine>(*pFork);
		m_upInputLine1->SetPipe(pFork->GetFirstOutgoing());
		m_upInputLine2->SetPipe(pFork->GetSecondOutgoing());
	}

	void Do() final
	{
		m_upFork = m_pNMWI->RemoveFromModel<Fork>(m_id);
		m_pNMWI->Push2Model(move(m_upInputLine1));
		m_pNMWI->Push2Model(move(m_upInputLine2));
	}

	void Undo() final
	{
		m_upInputLine2 = m_pNMWI->PopFromModel<InputLine>();
		m_upInputLine1 = m_pNMWI->PopFromModel<InputLine>();
		m_pNMWI->Restore2Model(move(m_upFork));
	}

private:
	NobId                 m_id;
	unique_ptr<InputLine> m_upInputLine1;
	unique_ptr<InputLine> m_upInputLine2;
	unique_ptr<Fork>      m_upFork;
};
