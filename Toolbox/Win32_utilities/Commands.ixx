// Commands.ixx
//
// Win32_utilities

module;

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <Windows.h>

export module Commands;

import Script;
import Scanner;
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

class Command;

export class CommandStack
{
public:

    virtual void Check() = 0;

    void Initialize(Observable* const);
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

    vector<unique_ptr<Command>> m_CommandStack { };
    size_t                      m_iIndex { 0 }; // index into m_Commandstack

    Observable* m_pStaticModelObservable { nullptr };

    Command* getCmdPtr(size_t const) const;
    Command& currentCmd () const;
    Command& previousCmd() const;

    void set2OlderCmd();
    void set2YoungerCmd();

    void notify() const;
    void clearRedoStack();
    bool canBeCombined(Command const*) const;
};

export class Command  // Implementation in Win32_Command.cpp
{
public:

    virtual ~Command() = default;

    virtual void Do();       
    virtual void Undo();     
    virtual void UpdateUI();

    virtual bool CombineCommands(Command const& src) { return false; };
    virtual bool IsAsyncCommand() { return false; };

    void CallUI(bool const); // called by Animation
    void TargetReached() { (m_targetReachedFunc)(); }

    static void Initialize(CommandStack* const);
    static void DoCall(WPARAM const, LPARAM const); // called by m_pWin
    static void NextScriptCommand();
    static void PushCommand(unique_ptr<Command> cmd) { m_pStack->PushCommand(move(cmd)); }
    static bool UndoCommand() { return m_pStack->UndoCommand(); }
    static bool RedoCommand() { return m_pStack->RedoCommand(); }
    static void ClearStack() { m_pStack->Clear(); }

protected:

    void AddPhase(unique_ptr<Command>);

    static bool      IsTraceOn()   { return m_bTrace; }
    static wostream& TraceStream() { return wcout; }

private:

    inline static bool           m_bTrace { true };
    inline static CommandStack * m_pStack { nullptr };

    function<void()>            m_targetReachedFunc { nullptr };
    vector<unique_ptr<Command>> m_phases            { };
    unsigned int                m_uiPhase           { 0 };

    bool m_bUndoMode { false };

    void doPhase();
    void undoPhase();

    void blockUI()   const;
    void unblockUI() const;
};

export void StartScript
(
    wstring       const& wstrFile,
    ScriptFunctor const& scriptHook
)
{
    wcout << Scanner::COMMENT_START + L"Processing script file " << wstrFile << endl;
    Script* pScript { ScriptStack::OpenScript() };
    if (pScript && pScript->ScrOpen(wstrFile))
    {
        pScript->ScrSetNewLineHook(&scriptHook);
        Command::NextScriptCommand();  // start reading script file
    }
}
