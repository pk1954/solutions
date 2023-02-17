// SplitPipeCommand.ixx
//
// Commands

module;

#include <memory>

export module SplitPipeCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class SplitPipeCommand : public NNetCommand
{
public:
	SplitPipeCommand(NobId const idPipe)
	  :	m_pPipe2Split(m_pNMWI->GetNobPtr<Pipe*>(idPipe))
	{}

	void InitSplit(PosNob & posNobSplit)
	{
		m_upPipeSplit1 = make_unique<Pipe>(Cast2PosNob(m_pPipe2Split->GetStartNobPtr()), &posNobSplit);
		m_upPipeSplit2 = make_unique<Pipe>(&posNobSplit, Cast2PosNob(m_pPipe2Split->GetEndNobPtr()));

		posNobSplit.AddIncoming(m_upPipeSplit1.get());
		posNobSplit.AddOutgoing(m_upPipeSplit2.get());

		bool const bSelected { m_pPipe2Split->IsSelected() };
		posNobSplit.    Select(bSelected);
		m_upPipeSplit1->Select(bSelected);
		m_upPipeSplit2->Select(bSelected);
	}

	~SplitPipeCommand() final = default;

	void Do() override
	{
		Cast2PosNob(m_pPipe2Split->GetStartNobPtr())->ReplaceOutgoing(m_pPipe2Split, m_upPipeSplit1.get());
		Cast2PosNob(m_pPipe2Split->GetEndNobPtr  ())->ReplaceIncoming(m_pPipe2Split, m_upPipeSplit2.get());
		m_pNMWI->Push2Model(move(m_upPipeSplit1));
		m_pNMWI->Push2Model(move(m_upPipeSplit2));
		m_upPipe2Split = m_pNMWI->RemoveFromModel<Pipe>(m_pPipe2Split->GetId());
	}

	void Undo() override
	{
		m_pNMWI->Restore2Model(move(m_upPipe2Split));
		m_upPipeSplit2 = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeSplit1 = m_pNMWI->PopFromModel<Pipe>();
		Cast2PosNob(m_pPipe2Split->GetEndNobPtr())  ->ReplaceIncoming(m_upPipeSplit2.get(), m_pPipe2Split);
		Cast2PosNob(m_pPipe2Split->GetStartNobPtr())->ReplaceOutgoing(m_upPipeSplit1.get(), m_pPipe2Split);
	}

private:

	Pipe     * const m_pPipe2Split;
	unique_ptr<Pipe> m_upPipe2Split;
	unique_ptr<Pipe> m_upPipeSplit1;
	unique_ptr<Pipe> m_upPipeSplit2;
};
