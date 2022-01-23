// NNetCommand.h
//
// Commands

#pragma once

#include "NobId.h"
#include "NNetModelWriterInterface.h"
#include "win32_command.h"

class BaseKnot;
class Pipe;

using std::unique_ptr;
using std::make_unique;

class NNetCommand : public Command
{
public:
    ~NNetCommand() override = default;

    static void Initialize(NNetModelWriterInterface * const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    virtual NobId GetAffectedNob() const
    {
        return NO_NOB;
    }

    unique_ptr<Pipe> MakePipe(BaseKnot* const p1, BaseKnot* const p2)
    {
        return make_unique<Pipe>(p1, p2, m_pNMWI->GetParams());
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};