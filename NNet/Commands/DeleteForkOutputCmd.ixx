// DeleteForkOutputCmd.ixx
//
// Commands
//
// handles only the two output Pipes of a Fork and the Fork itself
// not the input Pipe

module;

#include <memory>
#include <cassert>

export module NNetCommands:DeleteForkOutputCmd;

import :NNetCommand;

using std::unique_ptr;
using std::make_unique;

export class DeleteForkOutputCmd : public NNetCommand
{
public:

	explicit DeleteForkOutputCmd(Nob * const pNob)
		: m_pFork(Cast2Fork(pNob))
	{
		m_nobId = m_pFork->GetId();
		m_upInputLine1 = make_unique<InputLine>(*m_pFork);
		m_upInputLine2 = make_unique<InputLine>(*m_pFork);
		m_upInputLine1->SetPipe(m_pFork->GetFirstOutgoing());
		m_upInputLine2->SetPipe(m_pFork->GetSecondOutgoing());
	}

	void Do() final
	{
		m_upFork = m_pNMWI->RemoveFromModel<Fork>(m_nobId);
		assert(m_upFork);
		handleInputLine(m_upInputLine1);
		handleInputLine(m_upInputLine2);
	}

	void Undo() final
	{
		m_upInputLine2 = m_pNMWI->PopFromModel<InputLine>();
		m_upInputLine2->GetPipe()->SetStartPnt(m_upFork.get());
		m_upInputLine1 = m_pNMWI->PopFromModel<InputLine>();
		m_upInputLine1->GetPipe()->SetStartPnt(m_upFork.get());
		m_pNMWI->Restore2Model(move(m_upFork));
	}

private:
	NobId                 m_nobId;
	Fork*                 m_pFork;
	unique_ptr<InputLine> m_upInputLine1;
	unique_ptr<InputLine> m_upInputLine2;
	unique_ptr<Fork>      m_upFork;

	void handleInputLine(unique_ptr<InputLine> & upInputLine)
	{
		Pipe* pipe { upInputLine->GetPipe() };
		pipe->SetStartPnt(upInputLine.get());
		m_pNMWI->Push2Model(move(upInputLine));
	}
};
