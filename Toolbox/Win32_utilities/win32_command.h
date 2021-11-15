// win32_command.h
//
// Win32_utilities

#pragma once

#include <vector>

using std::vector;
using std::function;
using std::unique_ptr;

class RootWindow;

class Command
{
public:
    virtual ~Command() {}

    virtual void Do  ();
    virtual void Undo();
    virtual void UpdateUI();

    virtual bool const CombineCommands(Command const & src) 
    { 
        return false; 
    };

    virtual bool const IsAsyncCommand() 
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

private:

    inline static RootWindow  * m_pWin { nullptr };

    vector<unique_ptr<Command>> m_phases  { };  
    unsigned int                m_uiPhase { 0 };

    void doPhase();
    void undoPhase();

    void blockUI();
    void unblockUI();
};