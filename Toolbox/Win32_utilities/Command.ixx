// Command.ixx
//
// Win32_utilities

module;

#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <Windows.h>

export module Command;

import BaseCommand;

using std::unique_ptr;
using std::wostream;
using std::function;
using std::vector;
using std::wcout;

export class Command: public BaseCommand
{
public:

    virtual ~Command() = default;

    virtual void Do();
    virtual void Undo();
    
    void UpdateUI() override;

    virtual bool CombineCommands(Command const& src) { return false; };
    virtual bool IsAsyncCommand() { return false; };

    void CallUI(bool const); // called by Animation
    void TargetReached() 
    {
        if (m_targetReachedFunc)
            (m_targetReachedFunc)();
    }

    static void DoCall(WPARAM const, LPARAM const); // called by m_pWin
    static void NextScriptCommand();

protected:

    void AddPhase(unique_ptr<Command>);

    static bool      IsTraceOn  () { return m_bTrace; }
    static wostream& TraceStream() { return wcout; }

private:

    inline static bool m_bTrace { true };

    function<void()>            m_targetReachedFunc { nullptr };
    vector<unique_ptr<Command>> m_phases            { };
    unsigned int                m_uiPhase           { 0 };

    bool m_bUndoMode { false };

    void doPhase();
    void undoPhase();

    void blockUI()   const;
    void unblockUI() const;
};
