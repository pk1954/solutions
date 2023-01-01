// Commands.ixx
//
// Win32_utilities

module;

#include <vector>
#include <memory>
#include <iostream>
#include <functional>
#include <Windows.h>

export module Commands;

import Observable;
import RootWindow;

using std::vector;
using std::wcout;
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

export class Command
{
public:

    virtual ~Command() = default;

    virtual void Do();
    virtual void Undo();
    virtual void UpdateUI();

    virtual bool CombineCommands(Command const& src) { return false; };
    virtual bool IsAsyncCommand() { return false; };

    void CallUI(bool const); // called by Animation

    static void Initialize(RootWindow* const, CommandStack* const);
    static void DoCall(WPARAM const, LPARAM const); // called by m_pWin
    static void NextScriptCommand();

protected:

    void AddPhase(unique_ptr<Command>);

    function<void()> m_targetReachedFunc { nullptr };

    static bool      IsTraceOn()   { return m_bTrace; }
    static wostream& TraceStream() { return wcout; }

    static LRESULT PostCmd2Application(WPARAM const, LPARAM const);

    inline static CommandStack * m_pStack { nullptr };

private:

    inline static RootWindow * m_pWin   { nullptr };
    inline static bool         m_bTrace { true };

    vector<unique_ptr<Command>> m_phases    { };
    unsigned int                m_uiPhase   { 0 };
    bool                        m_bUndoMode { false };

    void doPhase();
    void undoPhase();

    void blockUI()   const;
    void unblockUI() const;
};