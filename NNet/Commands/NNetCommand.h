// NNetCommand.h
//
// Commands

#pragma once

#include "NNetModelWriterInterface.h"

import Command;
import NobId;

class NNetCommand : public Command
{
public:
    ~NNetCommand() override = default;

    static void SetModelInterface(NNetModelWriterInterface * const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    virtual NobId GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};