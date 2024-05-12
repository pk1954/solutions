// RenameSigGenCmd.ixx
//
// NNetCommands

module;

#include <string>
#include <memory>
#include <iostream>
#include <Windows.h>

export module NNetCommands:RenameSigGenCmd;

import EditLineBox;
import Signals;
import :NNetCommand;

export class RenameSigGenCmd : public NNetCommand
{
public:
	RenameSigGenCmd(SigGenId const id, wstring const& name)
	  : m_idSigGen(id),
		m_wstrNameOld(m_pNMWI->GetSigGenName(id)),
		m_wstrNameNew(name)
	{}

	void Do() final
	{
		m_pNMWI->SetSigGenName(m_idSigGen, m_wstrNameNew);
	}

	void Undo() final
	{
		m_pNMWI->SetSigGenName(m_idSigGen, m_wstrNameOld);
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Dialog(HWND const hwnd)
	{
		SigGenId const id       { m_pNMWI->GetSigGenIdSelected() };
		wstring        wstrName { m_pNMWI->GetSigGenName(id) };
		EditLineBox dlgBox(wstrName, L"Rename SignalGenerator");
		if (dlgBox.Show(hwnd))
			Push(id, wstrName);
	}

	static void Push(SigGenId const id, wstring const& name)
	{
		//if (IsTraceOn())  //TODO: fix
		//	TraceStream() << NAME << id.GetValue() << SPACE << Quoted(name) << endl;
		PushCommand(make_unique<RenameSigGenCmd>(id, name));
	}

private:

	inline static const wstring NAME { L"RenameSigGen" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			SigGenId const id   { ScrReadSigGenId(script) };
			script.ScrReadSpecial(L'"');
			wstring  const name { script.ScrReadString() };
			script.ScrReadSpecial(L'"');
			RenameSigGenCmd::Push(id, name);
		}
	} m_wrapper { NAME };

	SigGenId m_idSigGen;
	wstring  m_wstrNameOld;
	wstring  m_wstrNameNew;
};