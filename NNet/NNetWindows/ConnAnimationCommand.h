// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include <vector>
#include "IoNeuron.h"
#include "UPNobList.h"
#include "AnimationSequence.h"

using std::vector;

class WinCommands;
class RootWindow;

class ConnAnimationCommand : public AnimationSequence
{
public:
    ConnAnimationCommand(RootWindow  &, WinCommands &);
    virtual ~ConnAnimationCommand() {};

    bool const IsCmdOk() { return m_bAllOk; }

private:
    
    vector<IoNeuron *> m_nobsAnimated {};

    bool m_bAllOk { false };

    NobType const determineNobType(UPNobList const &) const;
};