// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include <vector>
#include "AnimationSequence.h"

using std::vector;

class WinCommands;
class MainWindow;

class ConnAnimationCommand : public AnimationSequence
{
public:
    ConnAnimationCommand(MainWindow  &, WinCommands &);
    virtual ~ConnAnimationCommand() {};

    virtual void Do();
    virtual void Undo();

    bool const IsCmdOk() { return m_bAllOk; }

private:
    
    vector<IoNeuron *> m_nobsAnimated {};

    bool m_bAllOk { false };

    NobType const determineNobType(UPNobList const &) const;
};