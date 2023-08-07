// AttachSigGen2ConCmd.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:AttachSigGen2ConCmd;

import :NNetCommand;

export class AttachSigGen2ConCmd : public NNetCommand
{
public:
	explicit AttachSigGen2ConCmd(NobId const nobId)
	  : m_inputConnector(* m_pNMWI->GetNobPtr<InputConnector *>(nobId)),
		m_pSigGenOld(m_inputConnector.GetSigGen())
	{}

	void Do() final
	{
		m_inputConnector.SetSigGen(m_pSigGenNew);
	}

	void Undo() final
	{
		m_inputConnector.SetSigGen(m_pSigGenOld);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId const id)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id << endl;
		PushCommand(make_unique<AttachSigGen2ConCmd>(id));
	}

private:

	inline static const wstring NAME { L"AttachSigGen2Con" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			AttachSigGen2ConCmd::Push(ScrReadNobId(script));
		}
	} m_wrapper { NAME };

	InputConnector  & m_inputConnector;
	SignalGenerator * m_pSigGenOld;
	SignalGenerator * m_pSigGenNew { m_pNMWI->GetSigGenSelected() };
};