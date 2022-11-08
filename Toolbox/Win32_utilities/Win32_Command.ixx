// Win32_Command.ixx
//
// Win32_utilities

module;

#include <vector>
#include <iostream>
#include <functional>
#include <source_location>
#include <Windows.h>

export module Command;

import RootWindow;

using std::wcout;
using std::vector;
using std::function;
using std::unique_ptr;
using std::source_location;
using std::wostream;

export class Command
{
public:
    virtual ~Command() = default;

    virtual void Do  ();
    virtual void Undo();
    virtual void UpdateUI();

    virtual bool CombineCommands(Command const & src) 
    { 
        return false; 
    };

    virtual bool IsAsyncCommand() 
    { 
        return false;
    };

    void CallUI(bool const); // called by Animation

    static void DoCall(WPARAM const, LPARAM const); // called by m_pWin

    static void Initialize(RootWindow * const pWin)
    {
        m_pWin = pWin;
    }

    static void NextScriptCommand();

protected:
    void AddPhase(unique_ptr<Command>);

    function<void()> m_targetReachedFunc { nullptr };

    static bool       IsTraceOn()   { return m_bTrace; }
    static wostream & TraceStream() { return wcout; }

    static LRESULT PostCmd2Application(WPARAM const wParam, LPARAM const lParam)
    {
        return m_pWin->PostCommand2Application(wParam, lParam);
    }

private:

    inline static RootWindow * m_pWin   { nullptr };
    inline static bool         m_bTrace { true };

    vector<unique_ptr<Command>> m_phases  { };  
    unsigned int                m_uiPhase { 0 };

    void doPhase();
    void undoPhase();

    void blockUI()   const;
    void unblockUI() const;
};