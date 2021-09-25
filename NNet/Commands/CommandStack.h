// CommandStack.h
//
// NNetModel

#pragma once

#include "assert.h"
#include <vector>
#include <utility>
#include <typeinfo>
#include <exception>
#include "BoolOp.h"
#include "Observable.h"
#include "win32_command.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

class NNetModelWriterInterface;

class CommandStack
{
public:

    void Initialize(NNetModelWriterInterface * const, Observable * const);

    bool UndoStackEmpty() const 
    { 
        return m_iIndex == 0; 
    }

    bool RedoStackEmpty() const 
    { 
        return m_iIndex == m_CommandStack.size(); 
    }

    void Push(unique_ptr<Command>);

    void PushCommand(unique_ptr<Command>);
    bool UndoCommand();
    bool RedoCommand();

    void Clear();

    void DoAll();
    void UndoAll();

private:

    vector<unique_ptr<Command>> m_CommandStack           { };
    size_t                          m_iIndex                 { 0 };     // index into m_Commandstack
    NNetModelWriterInterface      * m_pNMWI                  { nullptr };
    Observable                    * m_pStaticModelObservable { nullptr };

    Command & currentCmd () { return * m_CommandStack.at(m_iIndex); }
    Command & previousCmd() { return * m_CommandStack.at(m_iIndex-1); }; 

    void set2OlderCmd()
    {
        assert(! UndoStackEmpty());
        --m_iIndex;
    }

    void set2YoungerCmd()
    {
        assert(m_iIndex < m_CommandStack.size());
        ++m_iIndex;
    }

    void notify()
    {
        if (m_pStaticModelObservable)
            m_pStaticModelObservable->NotifyAll(true);
    }

    void clearRedoStack();

    bool const canBeCombined(Command const *);
};
