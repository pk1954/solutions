// ConnSynapse2NewPipeCmd.ixx
//
// Commands

module;

#include <iostream>

export module ConnSynapse2NewPipeCmd;

import NNetCommand;

export class ConnSynapse2NewPipeCmd : public NNetCommand
{
public:
	ConnSynapse2NewPipeCmd
	(
		NobId         const  idSynapse,
		NobId         const  idPipe,
		MicroMeterPnt const& umPntDelta
	)
	: m_pSynapse(m_pNMWI->GetNobPtr<Synapse*>(idSynapse)),
	  m_pPipeNew(m_pNMWI->GetNobPtr<Pipe*>(idPipe)),
	  m_pPipeOld(m_pSynapse->GetMainPipe()),
	  m_umPntDelta(umPntDelta)
	{}

	~ConnSynapse2NewPipeCmd() final = default;

	void Do() final
	{
		m_pSynapse->ChangeMainPipe(m_pPipeNew);
		m_pSynapse->MoveNob(m_umPntDelta);
	}

	void Undo() final
	{
		m_pSynapse->ChangeMainPipe(m_pPipeOld);
		m_pSynapse->MoveNob(-m_umPntDelta);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push
	(
		NobId         const  idSynapse,
		NobId         const  idPipe,
		MicroMeterPnt const& pos
	)
	{
		if (IsTraceOn())
			TraceStream() << NAME << idSynapse << idPipe << pos << endl;
		m_pStack->PushCommand(make_unique<ConnSynapse2NewPipeCmd>(idSynapse, idPipe, pos));
	}

private:

	inline static const wstring NAME { L"ConnSynapse2NewPipe" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId         const  idSynapse { ScrReadNobId(script) };
			NobId         const  idPipe    { ScrReadNobId(script) };
			MicroMeterPnt const& umPntNew  { ScrReadMicroMeterPnt(script) };
			ConnSynapse2NewPipeCmd::Push(idSynapse, idPipe, umPntNew);
		}
	};

	Synapse *           m_pSynapse;
	Pipe    *           m_pPipeOld;
	Pipe    *           m_pPipeNew; 
	MicroMeterPnt const m_umPntDelta;
};
