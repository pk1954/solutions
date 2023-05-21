// SetSigGenOriginCmd.ixx
//
// Commands

module;

#include <iostream>

export module NNetCommands:SetSigGenOriginCmd;

import IoUtil;
import NNetModel;
import :NNetCommand;

export class SetSigGenOriginCmd : public NNetCommand
{
public:
	SetSigGenOriginCmd(fPixelPoint const & fPixPnt)
	{ }

	void Do() final 
	{
	}

	void Undo() final
	{
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(fPixelPoint const& fPixPnt)
	{
		//if (IsTraceOn())        
		//	TraceStream() << NAME << fPixPnt << endl;
		m_pNMWI->GetSigGenList().SetSigGenOrigin(fPixPnt);
	}

private:

	inline static const wstring NAME { L"SetSigGenOrigin" };

	inline static struct Wrapper : public ScriptFunctor
	{
		void operator() (Script& script) const final
		{
			SetSigGenOriginCmd::Push(ScrReadfPixelPoint(script));
		}
	} m_wrapper;
};
