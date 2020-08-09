// Command.h
//
// NNetModel

#pragma once

#include "NNetModel.h"

class Command
{
public:

    virtual void Do( NNetModel * const ) = 0;
    
    virtual void Undo( NNetModel * const pModel ) 
    { 
        Do( pModel ); 
    };

    virtual bool IsMoveCommand( ) const
    { 
        return false; 
    };

};
