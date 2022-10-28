// CreateSynapseCommand.ixx
//
// Commands

module;

#include <memory>

export module CreateSynapseCommand;

import Types;
import NNetModel;
import NNetCommand;

using std::unique_ptr;
using std::make_unique;

export class CreateSynapseCommand : public NNetCommand
{
public:
	CreateSynapseCommand   // case 8
	(
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
      : m_idPipe(idPipe)
	{
		m_upSynapse   = make_unique<Synapse>(pos);
		m_upExtPoint  = make_unique<InputLine>(pos - m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho = make_unique<Pipe>(m_upExtPoint.get(), m_upSynapse.get());
		m_upExtPoint->AddOutgoing(*m_upPipeOrtho.get());
		m_upSynapse ->AddIncoming(*m_upPipeOrtho.get());

		m_pPipeOld   = m_pNMWI->GetNobPtr<Pipe*>(m_idPipe);
		m_pStartKnot = static_cast<BaseKnot*>(m_pPipeOld->GetStartKnotPtr());
		m_pEndKnot   = static_cast<BaseKnot*>(m_pPipeOld->GetEndKnotPtr());

		m_upPipeNew1 = make_unique<Pipe>(m_pStartKnot, m_upSynapse.get());
		m_upPipeNew2 = make_unique<Pipe>(m_upSynapse.get(), m_pEndKnot);

		m_upSynapse->AddIncoming(*m_upPipeNew1.get());
		m_upSynapse->AddOutgoing(*m_upPipeNew2.get());

		m_upSynapse ->Select(m_pPipeOld->IsSelected());
		m_upPipeNew1->Select(m_pPipeOld->IsSelected());
		m_upPipeNew2->Select(m_pPipeOld->IsSelected());
	}

	~CreateSynapseCommand() = default;

	void Do() final
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_upPipeNew1.get());
		m_pEndKnot->ReplaceIncoming(m_pPipeOld, m_upPipeNew2.get());

		m_pNMWI->Push2Model(move(m_upSynapse));
		m_pNMWI->Push2Model(move(m_upPipeNew1));
		m_pNMWI->Push2Model(move(m_upPipeNew2));
		m_pNMWI->Push2Model(move(m_upPipeOrtho));
		m_pNMWI->Push2Model(move(m_upExtPoint));

		m_upPipeOld = m_pNMWI->RemoveFromModel<Pipe>(m_idPipe);
	}

	void Undo() final
	{
		m_pNMWI->Restore2Model(move(m_upPipeOld));

		m_upExtPoint  = m_pNMWI->PopFromModel<IoLine>();
		m_upPipeOrtho = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew2  = m_pNMWI->PopFromModel<Pipe>();
		m_upPipeNew1  = m_pNMWI->PopFromModel<Pipe>();
		m_upSynapse   = m_pNMWI->PopFromModel<BaseKnot>();

		m_pEndKnot->ReplaceIncoming(m_upPipeNew2.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew1.get(), m_pPipeOld);
	}

private:

	Pipe               * m_pPipeOld    { nullptr };
	BaseKnot           * m_pStartKnot  { nullptr };
	BaseKnot           * m_pEndKnot    { nullptr };
	unique_ptr<Pipe>     m_upPipeOld   { nullptr };
	unique_ptr<Pipe>     m_upPipeNew1  { nullptr };
	unique_ptr<Pipe>     m_upPipeNew2  { nullptr };
	unique_ptr<BaseKnot> m_upSynapse   { nullptr };
	unique_ptr<IoLine>   m_upExtPoint  { nullptr };
	unique_ptr<Pipe>     m_upPipeOrtho { nullptr };
	NobId          const m_idPipe;
};
