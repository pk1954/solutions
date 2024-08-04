// CommandStack.cpp
//
// Toolbox\Commands

module Commands:CommandStack;

import std;
import Observable;
import Scanner;

using std::wcout;
using std::endl;
using std::function;
using std::unique_ptr;
using std::ranges::views::reverse;

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
    //assert(RedoStackEmpty());
}

BaseCommand* CommandStack::getCmdPtr(size_t const index) const
{
    BaseCommand* pCmd { m_CommandStack.at(index).get() };
    //assert(pCmd != nullptr);
    return pCmd;
}

BaseCommand& CommandStack::currentCmd() const
{
    return *getCmdPtr(m_iIndex);
}

BaseCommand& CommandStack::previousCmd() const
{
    return *getCmdPtr(m_iIndex - 1);
};

void CommandStack::set2OlderCmd()
{
    //assert(!UndoStackEmpty());
    --m_iIndex;
}

void CommandStack::set2YoungerCmd()
{
    //assert(m_iIndex < m_CommandStack.size());
    ++m_iIndex;
}

bool CommandStack::canBeCombined(BaseCommand const * pCmd) const
{
    if (UndoStackEmpty())
        return false;
    
    BaseCommand& prevCmd { previousCmd() };

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

void CommandStack::Push(unique_ptr<BaseCommand> pCmd)
{
    if (!canBeCombined(pCmd.get()))
    {
        m_CommandStack.push_back(move(pCmd));
        set2YoungerCmd();
    }
}

void CommandStack::PushStackCommand(unique_ptr<BaseCommand> pCmd)
{
    if (pCmd)
    {
        clearRedoStack();
        pCmd->Do();
        Check();
        if (!pCmd->IsAsyncCommand())
            pCmd->NextScriptCommand(); // script continuation for syncronous commands
        Push(move(pCmd));
        notify();
    }
    else
    {
        pCmd->NextScriptCommand(); // script continuation for failed commands
    }
}

bool CommandStack::UndoStackCommand()
{
    if (UndoStackEmpty())
        return false;
    set2OlderCmd();
    currentCmd().Undo();
    Check();
    notify();
    return true;
}

bool CommandStack::RedoStackCommand()
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
    for (auto & it : m_CommandStack)
        it->Do();
}

void CommandStack::UndoAll()
{
    for (auto & it : m_CommandStack|reverse)
        it->Undo();
}
