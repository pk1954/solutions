// Command.h
//
// NNetModel

#pragma once

#include "ShapeId.h"

class NNetModelWriterInterface;

class Command
{
public:
    virtual ~Command()
    {
        int x = 42;
    }

    virtual void Do( NNetModelWriterInterface & nmwi) 
    { 
    };
    
    virtual void Undo( NNetModelWriterInterface & nmwi )
    { 
        Do( nmwi ); 
    };

    virtual bool IsMoveCommand() const
    { 
        return false; 
    };

    virtual ShapeId const GetMovedShape() const
    {
        return ShapeId();
    }
};
