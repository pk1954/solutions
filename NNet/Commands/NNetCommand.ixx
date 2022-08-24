// NNetCommand.ixx
//
// Commands

module;

#include "NNetModelWriterInterface.h"
#include "NobId.h"

export module NNetCommand;

import Command;

export class NNetCommand : public Command
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