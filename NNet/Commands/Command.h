// Command.h
//
// NNetModel

#pragma once

class NNetModelWriterInterface;

class Command
{
public:
    virtual ~Command( )
    {
        int x = 42;
    }

    virtual void Do( NNetModelWriterInterface & ) 
    { 
    };
    
    virtual void Undo( NNetModelWriterInterface & model )
    { 
        Do( model ); 
    };

    virtual bool IsMoveCommand( ) const
    { 
        return false; 
    };
};
