// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include "AnimationSequence.h"

class WinCommands;
class MainWindow;

class ConnAnimationCommand : public AnimationSequence
{
public:
    ConnAnimationCommand(MainWindow  &, WinCommands &);
    virtual ~ConnAnimationCommand() {};

    bool const IsCmdOk() { return m_bAllOk; }

private:
    
    bool m_bAllOk { false };

    NobType const determineNobType(UPNobList const &) const;
};