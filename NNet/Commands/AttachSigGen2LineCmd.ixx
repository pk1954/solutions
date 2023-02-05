// AttachSigGen2LineCmd.ixx
//
// Commands

module;

#include <iostream>

export module AttachSigGen2LineCmd;

import NNetCommand;
import NNetModel;

export class AttachSigGen2LineCmd : public NNetCommand
{
public:
	explicit AttachSigGen2LineCmd(NobId const nobId)
	  : m_inputLine(*m_pNMWI->GetNobPtr<InputLine*>(nobId)),
		m_pSigGenOld(m_inputLine.GetSigGen())
	{}

	void Do() final
	{
		m_inputLine.SetSigGen(m_pSigGenNew);
	}

	void Undo() final
	{
		m_inputLine.SetSigGen(m_pSigGenOld);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(NobId const id)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << endl;
		m_pStack->PushCommand(make_unique<AttachSigGen2LineCmd>(id));
	}

private:

	inline static const wstring NAME { L"AttachSigGen2Line" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			AttachSigGen2LineCmd::Push(ScrReadNobId(script));
		}
	};

	InputLine       & m_inputLine;
	SignalGenerator * m_pSigGenOld;
	SignalGenerator * m_pSigGenNew{ m_pNMWI->GetSigGenSelected() };
};