// ExtendInputLineCmd.ixx
//
// Commands

module;

#include <memory>

export module ExtendInputLineCmd;

import Types;
import NNetCommand;
import NNetModel;

using std::make_unique;
using std::unique_ptr;

export class ExtendInputLineCmd : public NNetCommand
{
public:
	ExtendInputLineCmd         // case 10: Extend InputLine, adding a new knot    
	(                              
		NobId         const  id,       
		MicroMeterPnt const& pos
	)
	  : m_inputLineOld(*m_pNMWI->GetNobPtr<InputLine*>(id))
	{
		m_upPipe         = make_unique<Pipe>();
		m_upKnotNew      = make_unique<Knot>(m_inputLineOld.GetPos());
		m_upInputLineNew = make_unique<InputLine>(pos);
		ConnectOutgoing(*m_upPipe.get(), *m_upInputLineNew.get());
		ConnectIncoming(*m_upPipe.get(), *m_upKnotNew.get());
		m_upKnotNew->SetOutgoing(m_inputLineOld);
	}

	~ExtendInputLineCmd() final = default;

	void Do() final
	{
		m_inputLineOld.Apply2AllOutPipes([this](Pipe& p) { ConnectOutgoing(p, *m_upKnotNew.get()); });
		m_pNMWI->Push2Model(move(m_upKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upInputLineNew));
		m_upInputLineOld = m_pNMWI->RemoveFromModel<InputLine>(m_inputLineOld);
	}

	void Undo() final
	{
		m_upInputLineNew = m_pNMWI->PopFromModel<InputLine>();
		m_upPipe         = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotNew      = m_pNMWI->PopFromModel<Knot>();
		m_inputLineOld.Apply2AllOutPipes([this](Pipe& p) { ConnectOutgoing(p, *m_upInputLineOld.get()); });
		m_pNMWI->Restore2Model(move(m_upInputLineOld));
	}

private:

	InputLine      const& m_inputLineOld;
	unique_ptr<InputLine> m_upInputLineOld;
	unique_ptr<Knot>      m_upKnotNew;
	unique_ptr<InputLine> m_upInputLineNew;
	unique_ptr<Pipe>      m_upPipe;
};
