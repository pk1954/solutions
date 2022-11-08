// CreateForkCommand.ixx
//
// Commands

module;

#include <memory>

export module CreateForkCommand;

import Types;
import NNetModel;
import NNetCommand;

using std::unique_ptr;
using std::make_unique;
using std::pair;

export class CreateForkCommand : public NNetCommand
{
public:
	CreateForkCommand   // case 7 : Split Pipe, create fork and outgoing Pipe with OutputLine 
	(
		MicroMeterPnt const& pos,
		NobId         const  idPipe
	)
      : m_idPipe    (idPipe),
		m_pPipeOld  (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pStartKnot(static_cast<BaseKnot*>(m_pPipeOld->GetStartNobPtr())),
		m_pEndKnot  (static_cast<BaseKnot*>(m_pPipeOld->GetEndNobPtr()))
	{
		m_upFork       = make_unique<Fork>(pos);
		m_upOutputLine = make_unique<OutputLine>(pos + m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho  = make_unique<Pipe>(m_upFork.get(), m_upOutputLine.get());
		m_splitPipes   = m_pPipeOld->Split(*m_upFork.get());

		m_upFork->SetIncoming(m_splitPipes.first .get());
		m_upFork->SetOutgoing(m_splitPipes.second.get(), m_upPipeOrtho.get());

		m_upOutputLine->AddIncoming(*m_upPipeOrtho.get());
	}

	~CreateForkCommand() = default;

	void Do() final
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_splitPipes.first .get());
		m_pEndKnot  ->ReplaceIncoming(m_pPipeOld, m_splitPipes.second.get());

		m_splitPipes.first ->FixSynapses();
		m_splitPipes.second->FixSynapses();

		m_pNMWI->Push2Model(move(m_upFork));
		m_pNMWI->Push2Model(move(m_splitPipes.first ));
		m_pNMWI->Push2Model(move(m_splitPipes.second));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upOutputLine));

		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_upPipeOld->FixSynapses();

		m_pNMWI->Restore2Model(move(m_upPipeOld));

		m_upOutputLine      = m_pNMWI->PopFromModel<OutputLine>();
		m_upPipeOrtho       = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.second = m_pNMWI->PopFromModel<Pipe>();
		m_splitPipes.first  = m_pNMWI->PopFromModel<Pipe>();
		m_upFork            = m_pNMWI->PopFromModel<Fork>();

		m_pEndKnot  ->ReplaceIncoming(m_splitPipes.second.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_splitPipes.first .get(), m_pPipeOld);
	}

private:

	NobId const m_idPipe;
	Pipe      * m_pPipeOld;
	BaseKnot  * m_pStartKnot;  // start point of old Pipe
	BaseKnot  * m_pEndKnot;    // end point of old Pipe

	pair<unique_ptr<Pipe>, unique_ptr<Pipe>> m_splitPipes;

	unique_ptr<Pipe>       m_upPipeOld;
	unique_ptr<Fork>       m_upFork;
	unique_ptr<OutputLine> m_upOutputLine;
	unique_ptr<Pipe>       m_upPipeOrtho;
};
