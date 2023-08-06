// NewSigGenCmd.ixx
//
// Commands

module;

#include <memory>
#include <iostream>
#include "Resource.h"

export module NNetCommands:NewSigGenCmd;

import IoUtil;
import WinManager;
import Win32_Util_Resource;
import :NNetCommand;

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
		WinManager::PostCommand(RootWinId(IDM_SIG_DESIGNER), IDD_REGISTER_SIG_GEN, m_sigGenIdNew.GetValue());
        m_sigGenIdOld = m_pNMWI->SetSigGenActive(m_sigGenIdNew);
	}

	void Undo() final 
	{ 
		m_upSigGen = m_pNMWI->PopSigGen();
        m_pNMWI->SetSigGenActive(m_sigGenIdOld);
	}

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push()
    {
        if (IsTraceOn())
            TraceStream() << NAME << endl;
        PushCommand(make_unique<NewSigGenCmd>());
    }

private:

    inline static const wstring NAME { L"NewSigGen" };

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            NewSigGenCmd::Push();
        }
    } m_wrapper { NAME };

    UPSigGen m_upSigGen;
	SigGenId m_sigGenIdNew;
	SigGenId m_sigGenIdOld;
};