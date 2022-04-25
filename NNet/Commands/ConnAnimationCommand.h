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
class IoLine;

class ConnAnimationCommand : public NNetCommand
{
public:
    ConnAnimationCommand();

    bool IsAsyncCommand() final { return true; };

    bool IsCmdOk() const { return m_bAllOk; }

private:
    
    vector<IoLine *> m_nobsAnimated {};

    bool m_bAllOk { false };

    NobType determineNobType(UPNobList const &) const;
    void    sortNobsAnimated(MicroMeterLine const &);
};