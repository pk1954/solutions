// ExtendInputLineCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:ExtendInputLineCmd;

import IoUtil;
import :NNetCommand;

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

		m_upPipe->SetStartPnt(m_upInputLineNew.get());
		m_upPipe->SetEndPnt(m_upKnotNew.get());
		m_upPipe->PosChanged();

		m_upInputLineNew->SetPipe(m_upPipe.get());
		m_upKnotNew->AddIncoming(m_upPipe.get());
		m_upKnotNew->AddOutgoing(m_inputLineOld.GetPipe());
	}

	~ExtendInputLineCmd() final = default;

	void Do() final
	{
		m_upInputLineOld = m_pNMWI->RemoveFromModel<InputLine>(m_inputLineOld);
		m_inputLineOld.GetPipe()->SetStartPnt(m_upKnotNew.get());
		m_pNMWI->Push2Model(move(m_upKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upInputLineNew));
	}

	void Undo() final
	{
		m_upInputLineNew = m_pNMWI->PopFromModel<InputLine>();
		m_upPipe         = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotNew      = m_pNMWI->PopFromModel<Knot>();
		m_inputLineOld.GetPipe()->SetStartPnt(m_upInputLineOld.get());
		m_pNMWI->Restore2Model(move(m_upInputLineOld));
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const & pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << pos << endl;
		PushCommand(make_unique<ExtendInputLineCmd>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"ExtendInputLine" };

	inline static struct Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			ExtendInputLineCmd::Push(id, umPnt);
		}
	} m_wrapper;

	InputLine           & m_inputLineOld;
	unique_ptr<InputLine> m_upInputLineOld;
	unique_ptr<Knot>      m_upKnotNew;
	unique_ptr<InputLine> m_upInputLineNew;
	unique_ptr<Pipe>      m_upPipe;
};
