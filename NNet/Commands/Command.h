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
    {
        int x = 42;
    }

    virtual void Do(NNetModelWriterInterface & nmwi) 
    { 
    };
    
    virtual void Undo(NNetModelWriterInterface & nmwi)
    { 
        Do(nmwi); 
    };

    virtual bool const CombineCommands(Command const & src) 
    { 
        return false; 
    };

    virtual NobId const GetAffectedNob() const
    {
        return NobId();
    }
};
