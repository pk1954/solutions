// NNetCommand.h
//
// Commands

#pragma once

#include "NobId.h"
#include "win32_command.h"

class NNetModelWriterInterface;

class NNetCommand : public Command
{
public:
    virtual ~NNetCommand() {}

    NNetCommand() {}

    //NNetCommand(unique_ptr<NNetCommand> cmd)
    //{
    //    AddPhase(move(cmd));
    //}

    static void Initialize(NNetModelWriterInterface * const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    virtual NobId const GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};