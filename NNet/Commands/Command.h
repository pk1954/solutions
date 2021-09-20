// Command.h
//
// NNetModel

#pragma once

#include "NobId.h"
#include "BaseCommand.h"

class NNetModelWriterInterface;

class Command : public BaseCommand
{
public:
    virtual ~Command() {}

    static void Initialize(NNetModelWriterInterface* const pNMWI)
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
