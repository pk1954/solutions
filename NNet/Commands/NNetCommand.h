// NNetCommand.h
//
// Commands

#pragma once

#include "NobId.h"
#include "NNetModelWriterInterface.h"

import Command;

class BaseKnot;
class Pipe;

using std::unique_ptr;
using std::make_unique;

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