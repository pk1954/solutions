// ConnAnimationCommand.h
//
// Commands

#pragma once

#include <vector>
#include "UPNobList.h"
#include "NNetCommand.h"

using std::vector;

class WinCommands;
class RootWindow;
class IoNeuron;

class ConnAnimationCommand : public NNetCommand
{
public:
    ConnAnimationCommand();
    virtual ~ConnAnimationCommand() {};

    virtual bool const IsAsyncCommand() 
    { 
        return true; 
    };

    bool const IsCmdOk() { return m_bAllOk; }

private:
    
    vector<IoNeuron *> m_nobsAnimated {};

    bool m_bAllOk { false };

    NobType const determineNobType(UPNobList const &) const;
};