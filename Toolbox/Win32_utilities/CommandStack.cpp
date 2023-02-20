// CommandStack.cpp
//
// Win32_utilities

module;

#include <cassert>
#include <iostream>
#include <memory>
#include <functional>

module Commands;

import Observable;
import Scanner;

using std::wcout;
using std::endl;
using std::function;
using std::unique_ptr;

void CommandStack::Initialize(Observable * const pStaticModelObservable)
{
    m_pStaticModelObservable = pStaticModelObservable;
}

bool CommandStack::UndoStackEmpty() const 
{ 
    return m_iIndex == 0; 
}

bool CommandStack::RedoStackEmpty() const 
{ 
    return m_iIndex == m_CommandStack.size(); 
}

void CommandStack::Clear()
{
    m_iIndex = 0;
    clearRedoStack();
    notify();
}

void CommandStack::clearRedoStack()
{
    m_CommandStack.erase(m_CommandStack.begin() + m_iIndex, m_CommandStack.end());
    assert(RedoStackEmpty());
}

Command* CommandStack::getCmdPtr(size_t const index) const
{
    Command* pCmd { m_CommandStack.at(index).get() };
    assert(pCmd != nullptr);
    return pCmd;
}

Command& CommandStack::currentCmd() const
{
    return *getCmdPtr(m_iIndex);
}

Command& CommandStack::previousCmd() const
{
    return *getCmdPtr(m_iIndex - 1);
};

void CommandStack::set2OlderCmd()
{
    assert(!UndoStackEmpty());
    --m_iIndex;
}

void CommandStack::set2YoungerCmd()
{
    assert(m_iIndex < m_CommandStack.size());
    ++m_iIndex;
}

bool CommandStack::canBeCombined(Command const * pCmd) const
{
    if (UndoStackEmpty())
        return false;
    
    Command & prevCmd { previousCmd() };

    if (! (typeid(prevCmd) == typeid(*pCmd)) )
        return false;

    if (! prevCmd.CombineCommands(*pCmd))
        return false;

    return true;
}

void CommandStack::notify() const
{
    if (m_pStaticModelObservable)
        m_pStaticModelObservable->NotifyAll(true);
}

void CommandStack::Push(unique_ptr<Command> pCmd)
{
    if (!canBeCombined(pCmd.get()))
    {
        m_CommandStack.push_back(move(pCmd));
        set2YoungerCmd();
    }
}

void CommandStack::PushCommand(unique_ptr<Command> pCmd)
{
    if (pCmd)
    {
        clearRedoStack();
        pCmd->Do();
        Check();
        if (!pCmd->IsAsyncCommand())
            Command::NextScriptCommand(); // script continuation for syncronous commands
        Push(move(pCmd));
        notify();
    }
    else
    {
        Command::NextScriptCommand(); // script continuation for failed commands
    }
}

bool CommandStack::UndoCommand()
{
    if (UndoStackEmpty())
        return false;
    set2OlderCmd();
    currentCmd().Undo();
    Check();
    notify();
    return true;
}

bool CommandStack::RedoCommand()
{
    if (RedoStackEmpty()) 
        return false;
    currentCmd().Do();
    set2YoungerCmd();
    notify();
    return true;
}

void CommandStack::DoAll()
{
    for (size_t i = 0; i < m_CommandStack.size(); ++i)
        m_CommandStack[i]->Do();
}

void CommandStack::UndoAll()
{
    for (size_t i = m_CommandStack.size(); i --> 0;)
        m_CommandStack[i]->Undo();
}
