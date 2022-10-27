// AddPipe2PipeCommand.ixx
//
// Commands
//
// creates Forks and Synapses

module;

#include <memory>

export module AddPipe2PipeCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;
using std::make_unique;

export class AddPipe2PipeCommand : public NNetCommand
{
public:
	AddPipe2PipeCommand(NobId const idPipe)
		: m_idPipe(idPipe)
	{}

	~AddPipe2PipeCommand() final = default;

	void Do() final
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_upPipeNew1.get());
		m_pEndKnot  ->ReplaceIncoming(m_pPipeOld, m_upPipeNew2.get());

		m_pNMWI->Push2Model(move(m_upBaseKnotInsert));
		m_pNMWI->Push2Model(move(m_upPipeNew1));
		m_pNMWI->Push2Model(move(m_upPipeNew2));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upExtPoint));

		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upPipeOld));

		m_upExtPoint       = m_pNMWI->PopFromModel<IoLine>();
		m_upPipeOrtho      = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew2       = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew1       = m_pNMWI->PopFromModel<Pipe>();
		m_upBaseKnotInsert = m_pNMWI->PopFromModel<BaseKnot>();

		m_pEndKnot  ->ReplaceIncoming(m_upPipeNew2.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew1.get(), m_pPipeOld);
	}

protected:

	unique_ptr<BaseKnot> m_upBaseKnotInsert { nullptr };
	unique_ptr<IoLine>   m_upExtPoint       { nullptr };
	unique_ptr<Pipe>     m_upPipeOrtho      { nullptr };
	NobId          const m_idPipe;

	void Initialize()
	{
		m_pPipeOld   = m_pNMWI->GetNobPtr<Pipe*>(m_idPipe);
		m_pStartKnot = static_cast<BaseKnot*>(m_pPipeOld->GetStartKnotPtr());
		m_pEndKnot   = static_cast<BaseKnot*>(m_pPipeOld->GetEndKnotPtr());

		m_upPipeNew1 = make_unique<Pipe>(m_pStartKnot, m_upBaseKnotInsert.get());
		m_upPipeNew2 = make_unique<Pipe>(m_upBaseKnotInsert.get(), m_pEndKnot);

		m_upBaseKnotInsert->AddIncoming(*m_upPipeNew1.get());
		m_upBaseKnotInsert->AddOutgoing(*m_upPipeNew2.get());

		m_upBaseKnotInsert->Select(m_pPipeOld->IsSelected());
		m_upPipeNew1->Select(m_pPipeOld->IsSelected());
		m_upPipeNew2->Select(m_pPipeOld->IsSelected());
	}

private:
	Pipe           * m_pPipeOld   { nullptr };
	BaseKnot       * m_pStartKnot { nullptr };
	BaseKnot       * m_pEndKnot   { nullptr };
	unique_ptr<Pipe> m_upPipeOld  { nullptr };
	unique_ptr<Pipe> m_upPipeNew1 { nullptr };
	unique_ptr<Pipe> m_upPipeNew2 { nullptr };
};
