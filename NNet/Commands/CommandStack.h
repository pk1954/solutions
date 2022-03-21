// CommandStack.h
//
// NNetModel

#pragma once

#include "assert.h"
#include <vector>

using std::vector;
using std::unique_ptr;
using std::make_unique;

class Command;
class Observable;
class NNetModelReaderInterface;

class CommandStack
{
public:

    void Initialize(Observable * const);
    void SetModelInterface(NNetModelReaderInterface const * const);
    bool UndoStackEmpty() const; 
    bool RedoStackEmpty() const; 
    void Push(unique_ptr<Command>);
    void PushCommand(unique_ptr<Command>);
    bool UndoCommand();
    bool RedoCommand();
    void Clear();
    void DoAll();
    void UndoAll();

private:

    vector<unique_ptr<Command>>      m_CommandStack           { };
    size_t                           m_iIndex                 { 0 }; // index into m_Commandstack
    NNetModelReaderInterface const * m_pNMRI                  { nullptr };
    Observable                     * m_pStaticModelObservable { nullptr };

    Command & currentCmd () const { return * m_CommandStack.at(m_iIndex); }
    Command & previousCmd() const { return * m_CommandStack.at(m_iIndex-1); }; 

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

    void notify() const;
    void clearRedoStack();
    bool canBeCombined(Command const *) const;
};
