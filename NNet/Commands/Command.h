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

    virtual void Do( NNetModelWriterInterface * const) = 0;
    
    virtual void Undo( NNetModelWriterInterface * const pModel ) 
    { 
        Do( pModel ); 
    };

    virtual bool IsMoveCommand( ) const
    { 
        return false; 
    };
};
