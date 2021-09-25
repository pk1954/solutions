// CommandStack.cpp
//
// Commands

#include "stdafx.h"
#include "assert.h"
#include "NNetModelWriterInterface.h"
#include "CommandStack.h"

using std::wcout;
using std::endl;

void CommandStack::Initialize
(
    NNetModelWriterInterface * const pModel, 
    Observable               * const pStaticModelObservable 
)
{
    m_pNMWI = pModel;
    m_pStaticModelObservable = pStaticModelObservable;
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

void CommandStack::Push(unique_ptr<Command> pCmd)
{
    if (UndoStackEmpty() || ! (typeid(previousCmd()) == typeid(*pCmd)) || ! previousCmd().CombineCommands(*pCmd))
    {
        m_CommandStack.push_back(move(pCmd));
        set2YoungerCmd();
    }
}

void CommandStack::PushCommand(unique_ptr<Command> pCmd)
{
    if (pCmd)
    {
#ifdef _DEBUG
        //NNetModel const & model { m_pNMWI->GetModel() };
        //m_pNMWI->CheckModel();
        //NNetModel modelSave1(model);
        //m_pNMWI->CheckModel();
#endif
        clearRedoStack();
        m_pNMWI->CheckModel();
        pCmd->Do();
        m_pNMWI->CheckModel();
        Push(move(pCmd));
        notify();
#ifdef _DEBUG
        //NNetModel modelSave2(model);
        //modelSave2.CheckModel();
        //m_pNMWI->CheckModel();
        //UndoCommand();
        //m_pNMWI->CheckModel();
        //if (!(model == modelSave1))
        //{
        //    model.DUMP();
        //    modelSave1.DUMP();
        //    int x = 42;
        //}
        //m_pNMWI->CheckModel();
        //RedoCommand();
        //m_pNMWI->CheckModel();
        //if (!(model == modelSave2))
        //{
        //    int x = 42;
        //}
#endif
    }
}

bool CommandStack::UndoCommand()
{
    if (UndoStackEmpty())
       return false;
    set2OlderCmd();
    m_pNMWI->CheckModel();
    currentCmd().Undo();
    m_pNMWI->CheckModel();
    notify();
    return true;
}

bool CommandStack::RedoCommand()
{
    if (RedoStackEmpty()) 
        return false;
    m_pNMWI->CheckModel();
    currentCmd().Do();
    m_pNMWI->CheckModel();
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
