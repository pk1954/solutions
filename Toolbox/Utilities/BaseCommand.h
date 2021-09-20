// BaseCommand.h
//
// Utilities

#pragma once

#include <functional>

using std::function;

class BaseCommand
{
public:
    virtual void Do  () = 0;
    virtual void Undo() = 0;

    virtual bool const CombineCommands(BaseCommand const & src) 
    { 
        return false; 
    };
};