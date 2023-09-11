// SetScanAreaCmd.ixx
//
// NNetCommands

module;

#include <cassert>
#include <iostream>

export module NNetCommands:SetScanAreaCmd;

import Types;
import IoUtil;
import IoConstants;
import :NNetCommand;

export class SetScanAreaCmd : public NNetCommand
{
public:
	SetScanAreaCmd(MicroMeterRect const& rectNew)
	  : m_rectNew(rectNew),
		m_rectOld(m_pNMWI->GetScanAreaRect())
	{}

	void Do() final
	{
		m_pNMWI->SetScanArea(m_rectNew);
	}

	void Undo() final
	{
		m_pNMWI->SetScanArea(m_rectOld);
	}

	bool CombineCommands(BaseCommand const& src) final
	{
		SetScanAreaCmd const& cmdSrc { static_cast<SetScanAreaCmd const&>(src) };
		if (m_cardPnt != cmdSrc.m_cardPnt)
			return false;
		m_rectNew = cmdSrc.m_rectNew;
		return true;
	};

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(MicroMeterRect const& rect)
	{
		if (IsTraceOn())
			TraceStream() << NAME << rect << endl;
		PushCommand(make_unique<SetScanAreaCmd>(rect));
	}

private:

	inline static const wstring NAME { L"SetScanArea" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			SetScanAreaCmd::Push(ScrReadMicroMeterRect(script));
		}
	} m_wrapper { NAME };

	CardPoint      m_cardPnt;
	MicroMeterRect m_rectOld;
	MicroMeterRect m_rectNew;
};
