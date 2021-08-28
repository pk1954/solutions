// Command.h
//
// NNetModel

#pragma once

#include "NobId.h"

class NNetModelWriterInterface;

class Command
{
public:
    virtual ~Command()
    {}

    static void SetNNetModelWriterInterface(NNetModelWriterInterface* const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    virtual void Do  () = 0;
    virtual void Undo() = 0;

    virtual bool const CombineCommands(Command const & src) 
    { 
        return false; 
    };

    virtual NobId const GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};
