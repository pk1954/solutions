// StdStackCommand.cpp
//
// Toolbox\Commands

module;

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <iostream>

module StdStackCommand;

import SaveCast;
import SoundInterface;

using std::unique_ptr;

void StdStackCommand::TargetReached()
{
    if (m_targetReachedFunc)
        (m_targetReachedFunc)(this);
}

void StdStackCommand::Do()
{
    UpdateUI();
}

void StdStackCommand::Undo()
{
    UpdateUI();
}
