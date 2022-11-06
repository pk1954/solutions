// ConnectCreateForkCommand.ixx
//
// Commands

module;

#include <cassert>
#include <memory>

export module ConnectCreateForkCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;
using std::pair;

export class ConnectCreateForkCommand : public NNetCommand
{
public:
	ConnectCreateForkCommand   // case 1
	(
		NobId const idIoLine,
		NobId const idPipe
	)
	  : m_idIoLine  (idIoLine),
		m_idPipe    (idPipe),
		m_pInputLine(m_pNMWI->GetNobPtr<InputLine*>(idIoLine)),
		m_pPipeOld  (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pStartKnot(static_cast<BaseKnot*>(m_pPipeOld->GetStartKnotPtr())),
		m_pEndKnot  (static_cast<BaseKnot*>(m_pPipeOld->GetEndKnotPtr()))
	{
		m_splitPipes = m_pPipeOld->Split(*m_pInputLine);

		m_upFork = make_unique<Fork>(m_pInputLine->GetPos());
		m_upFork->SetIncoming(m_splitPipes.first .get());
		m_upFork->SetOutgoing(m_splitPipes.second.get(), &m_pInputLine->GetPipe());

		m_upFork           ->Select(m_pPipeOld->IsSelected());
		m_splitPipes.first ->Select(m_pPipeOld->IsSelected());
		m_splitPipes.second->Select(m_pPipeOld->IsSelected());
	}

	~ConnectCreateForkCommand() final = default;

	void Do() final
	{
		m_pInputLine->GetPipe().SetStartPnt(m_upFork.get());

		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_splitPipes.first .get());
		m_pEndKnot  ->ReplaceIncoming(m_pPipeOld, m_splitPipes.second.get());

		m_splitPipes.first ->FixSynapses();
		m_splitPipes.second->FixSynapses();

		m_pNMWI->Push2Model(move(m_upFork));
		m_pNMWI->Push2Model(move(m_splitPipes.first ));
		m_pNMWI->Push2Model(move(m_splitPipes.second));

		m_upIoLine  = m_pNMWI->RemoveFromModel<InputLine>(m_idIoLine);
		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>     (m_idPipe);
	}

	void Undo() final
	{
		m_upPipeOld->FixSynapses();

		m_pNMWI->Restore2Model(move(m_upPipeOld));
		m_pNMWI->Restore2Model(move(m_upIoLine));

		m_splitPipes.second = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first  = m_pNMWI->PopFromModel<Pipe>();
		m_upFork            = m_pNMWI->PopFromModel<Fork>();

		m_pEndKnot  ->ReplaceIncoming(m_splitPipes.second.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_splitPipes.first .get(), m_pPipeOld);

		m_pInputLine->GetPipe().SetStartPnt(m_pInputLine);
	}

private:
	NobId       const m_idIoLine;
	NobId       const m_idPipe;
	InputLine * const m_pInputLine;
	Pipe      * const m_pPipeOld;
	BaseKnot  * const m_pStartKnot;
	BaseKnot  * const m_pEndKnot;

	pair<unique_ptr<Pipe>, unique_ptr<Pipe>> m_splitPipes;

	unique_ptr<Pipe>      m_upPipeOld;
	unique_ptr<Fork>      m_upFork;
	unique_ptr<InputLine> m_upIoLine;
};
