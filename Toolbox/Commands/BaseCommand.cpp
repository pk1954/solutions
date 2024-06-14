// BaseCommand.cpp
//
// Toolbox\Commands

module;

#include <memory>
#include <functional>
#include <vector>
#include <string>
#include <iostream>

module Commands:BaseCommand;

import SaveCast;

using std::unique_ptr;

void BaseCommand::TargetReached()
{
    if (m_targetReachedFunc)
        (m_targetReachedFunc)(this);
}

void BaseCommand::Do()
{
    UpdateUI();
}

void BaseCommand::Undo()
{
    UpdateUI();
}
