// InsertPosNobCommand.ixx
//
// Commands

module;

#include <memory>

export module InsertPosNobCommand;

import Types;
import NNetCommand;
import NNetModel;

using std::unique_ptr;

export template <PosNob_t T>
class InsertPosNobCommand : public NNetCommand
{
public:
	InsertPosNobCommand
	(
		NobId         const   idPipe, 
		MicroMeterPnt const & umSplitPoint 
	)
	  :	m_idPipe(idPipe),
		m_umSplitPoint(umSplitPoint)
	{ 
		m_pPipe2Split = m_pNMWI->GetNobPtr<Pipe *>(m_idPipe);
		m_pStartNob   = static_cast<PosNob *>(m_pPipe2Split->GetStartNobPtr());
		m_upInsertNob = make_unique<T>(m_umSplitPoint);
		m_upPipeNew   = make_unique<Pipe>(m_pStartNob, m_upInsertNob.get());
		m_upInsertNob->AddOutgoing(*m_pPipe2Split);
		m_upInsertNob->AddIncoming(*m_upPipeNew.get());
	}

	~InsertPosNobCommand() final = default;

	void Do() final 
	{ 
		m_pStartNob->ReplaceOutgoing(m_pPipe2Split, m_upPipeNew.get());
		m_pPipe2Split->SetStartPnt(m_upInsertNob.get());
		m_pNMWI->Push2Model(move(m_upInsertNob));
		m_pNMWI->Push2Model(move(m_upPipeNew));
	}

	void Undo() final 
	{ 
		m_upPipeNew   = m_pNMWI->PopFromModel<Pipe>();
		m_upInsertNob = m_pNMWI->PopFromModel<T>();
		m_pPipe2Split->SetStartPnt(m_pStartNob);
		m_pStartNob->ReplaceOutgoing(m_upPipeNew.get(), m_pPipe2Split);
	}

private:
	Pipe   * m_pPipe2Split { nullptr };
	PosNob * m_pStartNob  { nullptr };

	unique_ptr<Pipe> m_upPipeNew;
	unique_ptr<T>    m_upInsertNob;

	NobId         const m_idPipe;
	MicroMeterPnt const m_umSplitPoint; 
};
