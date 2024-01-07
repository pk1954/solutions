// SizeSelectionCmd.ixx
//
// NNetCommands

module;

#include <iostream>

export module NNetCommands:SizeSelectionCmd;

import :NNetCommand;
import SaveCast;

export class SizeSelectionCmd : public NNetCommand
{
public:
	explicit SizeSelectionCmd(float const fFactor)
		: m_fFactor(fFactor)
	{
		m_umPntCenter = m_pNMWI->GetUPNobs().CenterOfGravity
		(
			[](Nob const & nob){ return nob.IsSelected(); }
		);
	}

	void Do  () final { sizeSelection(m_fFactor); }
	void Undo() final { sizeSelection(1.0f / m_fFactor); }

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(float const fFactor)
	{
		if (IsTraceOn())
			TraceStream() << NAME << fFactor << endl;
		PushCommand(make_unique<SizeSelectionCmd>(fFactor));
	}

private:

	inline static const wstring NAME { L"SizeSelection" };

	inline static struct myWrapper : public Wrapper
	{
		using Wrapper::Wrapper;
		void operator() (Script& script) const final
		{
			SizeSelectionCmd::Push(Cast2Float(script.ScrReadFloat()));
		}
	} m_wrapper { NAME };

	void sizeSelection(float const fFactor) const
	{
		m_pNMWI->GetUPNobs().Apply2AllSelected<PosNob>
		(
			[this, fFactor](PosNob & b) 
			{ 
				b.SetPos(m_umPntCenter + (b.GetPos() - m_umPntCenter) * fFactor);
			}
		);
	}

	MicroMeterPnt m_umPntCenter;
	float         m_fFactor;
};
