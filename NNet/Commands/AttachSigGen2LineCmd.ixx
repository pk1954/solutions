// AttachSigGen2LineCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:AttachSigGen2LineCmd;

import :NNetCommand;
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
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const id)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id.GetValue() << endl;
		PushCommand(make_unique<AttachSigGen2LineCmd>(id));
	}

private:

	inline static const wstring NAME { L"AttachSigGen2Line" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			AttachSigGen2LineCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper;

	InputLine       & m_inputLine;
	SignalGenerator * m_pSigGenOld;
	SignalGenerator * m_pSigGenNew{ m_pNMWI->GetSigGenSelected() };
};