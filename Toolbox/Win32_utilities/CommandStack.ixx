// CommandStack.ixx
//
// Win32_utilities

module;

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <Windows.h>

export module CommandStack;

import Script;
import Scanner;
import Command;
import Observable;
import RootWindow;
import ScriptStack;

using std::wstring;
using std::vector;
using std::wcout;
using std::endl;
using std::wostream;
using std::function;
using std::unique_ptr;

export class CommandStack
{
public:

    virtual void Check() = 0;

    void Initialize(Observable* const);
    bool UndoStackEmpty() const;
    bool RedoStackEmpty() const;
    void Push(unique_ptr<Command>);
    void PushStackCommand(unique_ptr<Command>);
    bool UndoStackCommand();
    bool RedoStackCommand();
    void Clear();
    void DoAll();
    void UndoAll();

private:

    vector<unique_ptr<Command>> m_CommandStack { };
    size_t                      m_iIndex { 0 }; // index into m_Commandstack

    Observable* m_pStaticModelObservable { nullptr };

    Command* getCmdPtr(size_t const) const;
    Command& currentCmd() const;
    Command& previousCmd() const;

    void set2OlderCmd();
    void set2YoungerCmd();

    void notify() const;
    void clearRedoStack();
    bool canBeCombined(Command const*) const;
};

