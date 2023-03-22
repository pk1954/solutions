// NewSigGenCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>
#include "Resource.h"

export module NewSigGenCmd;

import NNetCommand;

export class NewSigGenCmd : public NNetCommand
{
public:
	NewSigGenCmd()
	{
		m_upSigGen = m_pNMWI->NewSigGen();
	}

	void Do() final 
	{ 
		m_sigGenIdNew = m_pNMWI->PushSigGen(move(m_upSigGen));
		PostCmd2Application(IDD_REGISTER_SIG_GEN, m_sigGenIdNew.GetValue());
        m_pNMWI->SetSigGenActive(m_sigGenIdNew);
	}

	void Undo() final 
	{ 
		m_upSigGen = m_pNMWI->PopSigGen();
        m_pNMWI->SetSigGenActive(m_sigGenIdOld);
	}

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, new Wrapper);
    }

    static void Push()
    {
        if (IsTraceOn())
            TraceStream() << NAME << endl;
        m_pStack->PushCommand(make_unique<NewSigGenCmd>());
    }

private:

    inline static const wstring NAME { L"NewSigGen" };

    class Wrapper : public ScriptFunctor
    {
    public:
        void operator() (Script& script) const final
        {
            NewSigGenCmd::Push();
        }
    };

    UPSigGen m_upSigGen;
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};