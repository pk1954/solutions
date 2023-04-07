// SetNobCommand.ixx
//
// Commands

module;

#include <iostream>

export module SetNobCommand;

import Types;
import NNetCommand;
import NNetModel;

export class SetNobCommand : public NNetCommand
{
public:
	SetNobCommand
	(
		Nob                    &nob, 
		MicroMeterPosDir const &posDir
	)
	  : m_nob(nob),
		m_posDir(posDir)
	{}

	void Do() final 
	{ 
		m_nob.RotateNob(m_nob .GetPos(), m_posDir.GetDir());
		m_nob.MoveNob  (m_posDir.GetPos() - m_nob.GetPos());
	}

	void Undo() final 
	{ 
		m_nob.RotateNob(m_nob.GetPos(), -m_posDir.GetDir());
		m_nob.MoveNob  (m_nob.GetPos() - m_posDir.GetPos());
	}

	static void Register()
	{
		SymbolTable::ScrDefConst(NAME, &m_wrapper);
	}

	static void Push(NobId nobId, MicroMeterPosDir const& posDir)
	{
		if (IsTraceOn())
			TraceStream() << NAME << nobId.GetValue() << posDir << endl;
		m_pStack->PushCommand(make_unique<SetNobCommand>(*m_pNMWI->GetNob(nobId), posDir));
	}

private:

	inline static const wstring NAME { L"SetNob" };

	inline static struct Wrapper : public ScriptFunctor
	{
	public:
		void operator() (Script& script) const final
		{
			NobId            const nobId    { ScrReadNobId(script) };
			MicroMeterPosDir const umPosDir { ScrReadMicroMeterPosDir(script) };
			SetNobCommand::Push(nobId, umPosDir);
		}
	} m_wrapper;

	MicroMeterPosDir const m_posDir;
	Nob                  & m_nob;
};
