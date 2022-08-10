// CommandStack.cpp
//
// Commands

#include <cassert>
#include <source_location>
#include "assert.h"
#include "NobException.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"

import Observable;
import Command;

using std::wcout;
using std::endl;
using std::source_location;

void CommandStack::Initialize(Observable * const pStaticModelObservable)
{
    m_pStaticModelObservable = pStaticModelObservable;
}

void CommandStack::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
{
    m_pNMRI = pNMRI;
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
    try
    {
        if (!canBeCombined(pCmd.get()))
        {
            m_CommandStack.push_back(move(pCmd));
            set2YoungerCmd();
        }
    }
    catch (NNetException const& e)
    {
        NNetExceptionMessage(e);
        wcout << Scanner::COMMENT_SYMBOL << L"Called from" << source_location::current().function_name() << endl;
    }
}

void CommandStack::PushCommand(unique_ptr<Command> pCmd)
{
    if (pCmd)
    {
        clearRedoStack();
        m_pNMRI->CheckModel();
        pCmd->Do();
        if (!pCmd->IsAsyncCommand())
            Command::NextScriptCommand(); // script continuation for syncronous commands
        m_pNMRI->CheckModel();
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
    try
    {
        if (UndoStackEmpty())
            return false;
        set2OlderCmd();
        m_pNMRI->CheckModel();
        currentCmd().Undo();
        m_pNMRI->CheckModel();
        notify();
    }
    catch (NNetException const& e)
    {
        NNetExceptionMessage(e);
        wcout << Scanner::COMMENT_SYMBOL << L"Called from" << source_location::current().function_name() << endl;
    }
    return true;
}

bool CommandStack::RedoCommand()
{
    try
    {
        if (RedoStackEmpty()) 
            return false;
        m_pNMRI->CheckModel();
        currentCmd().Do();
        m_pNMRI->CheckModel();
        set2YoungerCmd();
        notify();
    }
    catch (NNetException const& e)
    {
        NNetExceptionMessage(e);
        wcout << Scanner::COMMENT_SYMBOL << L"Called from" << source_location::current().function_name() << endl;
    }
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
