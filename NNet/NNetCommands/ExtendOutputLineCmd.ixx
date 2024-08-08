// ExtendOutputLineCmd.ixx
//
// NNetCommands

export module NNetCommands:ExtendOutputLineCmd;

import std;
import IoUtil;
import :NNetCommand;

export class ExtendOutputLineCmd : public NNetCommand
{
public:
	ExtendOutputLineCmd      // case 11: Extend Outputline, adding a new knot       
	(                               
		NobId         const   id,   
		MicroMeterPnt const & pos    
	)
      :	m_outputLineOld(*m_pNMWI->GetNobPtr<OutputLine*>(id))
	{
		MicroMeterPnt const& posOld { m_outputLineOld.GetPos() };
		m_upPipe          = make_unique<Pipe>();
		m_upKnotNew       = make_unique<Knot>(posOld);
		m_upOutputLineNew = make_unique<OutputLine>(posOld);

		m_upPipe->SetStartPnt(m_upKnotNew.get());
		m_upPipe->SetEndPnt  (m_upOutputLineNew.get());
		m_upPipe->PosChanged();
		
		m_upOutputLineNew->SetPipe(m_upPipe.get());
		m_upKnotNew->AddOutgoing(m_upPipe.get());
		m_upKnotNew->AddIncoming(m_outputLineOld.GetPipe());

		m_upOutputLineNew->SetPos(pos);
	}

	~ExtendOutputLineCmd() final = default;

	void Do() final 
	{ 
		m_upOutputLineOld = m_pNMWI->RemoveFromModel<OutputLine>(m_outputLineOld);
		m_upOutputLineOld->GetPipe()->SetEndPnt(m_upKnotNew.get());
		m_pNMWI->Push2Model(move(m_upKnotNew));
		m_pNMWI->Push2Model(move(m_upPipe));
		m_pNMWI->Push2Model(move(m_upOutputLineNew));
	}

	void Undo() final 
	{ 
		m_upOutputLineNew = m_pNMWI->PopFromModel<OutputLine>();
		m_upPipe          = m_pNMWI->PopFromModel<Pipe>();
		m_upKnotNew       = m_pNMWI->PopFromModel<Knot>();
		m_outputLineOld.GetPipe()->SetEndPnt(m_upOutputLineOld.get());
		m_pNMWI->Restore2Model(move(m_upOutputLineOld));
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId, MicroMeterPnt const& pos)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId << pos << endl;
		PushCommand(make_unique<ExtendOutputLineCmd>(nobId, pos));
	}

private:

	inline static const wstring NAME { L"ExtendOutputLine" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			NobId         const id    { ScrReadNobId(script) };
			MicroMeterPnt const umPnt { ScrReadMicroMeterPnt(script) };
			ExtendOutputLineCmd::Push(id, umPnt);
		}
	} m_wrapper { NAME };

	OutputLine           & m_outputLineOld;
	unique_ptr<OutputLine> m_upOutputLineOld;
	unique_ptr<Knot>       m_upKnotNew;
	unique_ptr<OutputLine> m_upOutputLineNew;
	unique_ptr<Pipe>       m_upPipe;
};
