// win32_command.h
//
// Win32_utilities

#pragma once

#include <vector>
#include <functional>
#include <source_location>

using std::vector;
using std::function;
using std::unique_ptr;
using std::source_location;

class RootWindow;

class Command
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

    LRESULT PostCommand2Application(WPARAM const, LPARAM const) const;
        
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

    void blockUI()   const;
    void unblockUI() const;
};