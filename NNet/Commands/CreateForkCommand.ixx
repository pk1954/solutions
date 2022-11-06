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

export class CreateForkCommand : public NNetCommand
{
public:
	CreateForkCommand   // case 7
	(
		MicroMeterPnt const& pos,
		NobId         const  idPipe
	)
      : m_idPipe    (idPipe),
		m_pPipeOld  (m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
		m_pStartKnot(static_cast<BaseKnot*>(m_pPipeOld->GetStartKnotPtr())),
		m_pEndKnot  (static_cast<BaseKnot*>(m_pPipeOld->GetEndKnotPtr()))
	{
		m_upFork      = make_unique<Fork>(pos);
		m_upExtPoint  = make_unique<OutputLine>(pos + m_pNMWI->OrthoVector(m_idPipe));
		m_upPipeOrtho = make_unique<Pipe>(m_upFork.get(), m_upExtPoint.get());

		m_upPipeNew1 = make_unique<Pipe>(m_pStartKnot, m_upFork.get());
		m_upPipeNew2 = make_unique<Pipe>(m_upFork.get(), m_pEndKnot);

		m_upFork->SetIncoming(m_upPipeNew1.get());
		m_upFork->SetOutgoing(m_upPipeNew2.get(), m_upPipeOrtho.get());

		m_upExtPoint->AddIncoming(*m_upPipeOrtho.get());

		m_upFork    ->Select(m_pPipeOld->IsSelected());
		m_upPipeNew1->Select(m_pPipeOld->IsSelected());
		m_upPipeNew2->Select(m_pPipeOld->IsSelected());
	}

	~CreateForkCommand() = default;

	void Do() final
	{
		m_pStartKnot->ReplaceOutgoing(m_pPipeOld, m_upPipeNew1.get());
		m_pEndKnot  ->ReplaceIncoming(m_pPipeOld, m_upPipeNew2.get());

		m_pNMWI->Push2Model(move(m_upFork));
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
		m_upFork = m_pNMWI->PopFromModel<Fork>();

		m_pEndKnot  ->ReplaceIncoming(m_upPipeNew2.get(), m_pPipeOld);
		m_pStartKnot->ReplaceOutgoing(m_upPipeNew1.get(), m_pPipeOld);
	}

private:

	Pipe             * m_pPipeOld    { nullptr };
	BaseKnot         * m_pStartKnot  { nullptr };
	BaseKnot         * m_pEndKnot    { nullptr };
	unique_ptr<Pipe>   m_upPipeOld   { nullptr };
	unique_ptr<Pipe>   m_upPipeNew1  { nullptr };
	unique_ptr<Pipe>   m_upPipeNew2  { nullptr };
	unique_ptr<Fork>   m_upFork      { nullptr };
	unique_ptr<IoLine> m_upExtPoint  { nullptr };
	unique_ptr<Pipe>   m_upPipeOrtho { nullptr };
	NobId        const m_idPipe;
};
