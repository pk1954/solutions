// RenameSigGenCmd.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module RenameSigGenCmd;

import NNetCommand;

export class RenameSigGenCmd : public NNetCommand
{
public:
	RenameSigGenCmd(SigGenId const id, wstring const& name)
	  : m_pSigGen(m_pNMWI->GetSigGen(id)),
		m_wstrNameOld(m_pSigGen->GetName()),
		m_wstrNameNew(name)
	{}

	void Do() final
	{
		m_pSigGen->SetName(m_wstrNameNew);
	}

	void Undo() final
	{
		m_pSigGen->SetName(m_wstrNameOld);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, new Wrapper);
	}

	static void Push(SigGenId const id, wstring const& name)
	{
		if (IsTraceOn())
			TraceStream() << NAME << id.GetValue() << L" \"" << name << L"\" " << endl;
		m_pStack->PushCommand(make_unique<RenameSigGenCmd>(id, name));
	}

private:

	inline static const wstring NAME { L"RenameSigGen" };

	class Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			SigGenId const id   { ScrReadSigGenId(script) };
			script.ScrReadSpecial(L'"');
			wstring  const name { script.ScrReadString() };
			script.ScrReadSpecial(L'"');
			RenameSigGenCmd::Push(id, name);
		}
	};

	SignalGenerator* m_pSigGen;
	wstring           m_wstrNameOld;
	wstring           m_wstrNameNew;
};