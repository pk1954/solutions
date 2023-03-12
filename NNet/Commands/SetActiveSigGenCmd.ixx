// SetActiveSigGenCmd.ixx
//
// Commands

module;

#include <iostream>

export module SetActiveSigGenCmd;

import SigGenCommand;
import NNetModel;

export class SetActiveSigGenCmd : public SigGenCommand
{
public:
	SetActiveSigGenCmd(SigGenId const id)
	{
		m_sigGenIdNew = id;
	}

	void Do() final
	{
		m_sigGenIdOld = SetActiveSigGenId(m_sigGenIdNew);
	}

	void Undo() final
	{
		SetActiveSigGenId(m_sigGenIdOld);
	}

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, new Wrapper);
    }

    static void Push(SigGenId const id)
    {
        if (IsTraceOn())
            TraceStream() << NAME << L' ' << id.GetValue() << endl;
        m_pStack->PushCommand(make_unique<SetActiveSigGenCmd>(id));
    }

private:

    inline static const wstring NAME { L"SetActiveSigGen" };

    class Wrapper : public ScriptFunctor
    {
    public:
        void operator() (Script& script) const final
        {
            SetActiveSigGenCmd::Push(ScrReadSigGenId(script));
        }
    };

    SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};