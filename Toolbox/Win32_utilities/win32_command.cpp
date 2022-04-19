// win32_command.cpp
//
// Win32_utilities

#include "stdafx.h"
#include "ScriptStack.h"
#include "win32_util_resource.h"
#include "win32_rootWindow.h"
#include "win32_command.h"

using std::bit_cast;

void Command::UpdateUI() 
{ 
    m_pWin->Notify(false); 
};

LRESULT Command::PostCommand2Application(WPARAM const wParam, LPARAM const lParam) const
{
    return m_pWin->PostCommand2Application(wParam, lParam);
}

void Command::CallUI(bool const bTargetReached)  // runs in animation thread
{
    PostMessage
    (
        m_pWin->GetWindowHandle(),
        WM_APP_UI_CALL,            // calls DoCall from UI thread
        static_cast<WPARAM>(bTargetReached), 
        bit_cast<LPARAM>(this)
    );
}

void Command::DoCall(WPARAM const wParam, LPARAM const lParam) // runs in UI thread
{
    Command * const pAnimCmd       { bit_cast<Command * const>(lParam) };
    bool      const bTargetReached { static_cast<bool const>(wParam) };
    pAnimCmd->UpdateUI();
    if (bTargetReached && pAnimCmd->m_targetReachedFunc)
        (pAnimCmd->m_targetReachedFunc)(); 
};

void Command::doPhase() // runs in UI thread
{
    if (m_uiPhase == 0)
        blockUI();
    if (m_uiPhase < m_phases.size())
    {
        Command * const pAnimCmd { m_phases[m_uiPhase++].get() };
        pAnimCmd->m_targetReachedFunc = [&](){ doPhase(); };
        pAnimCmd->Do();
    }
    else
        unblockUI();
    UpdateUI();
}

void Command::undoPhase() // runs in UI thread
{
    if (m_uiPhase >= m_phases.size())
        blockUI();
    if (m_uiPhase > 0)
    {
        Command & animCmd { * m_phases[--m_uiPhase] };
        animCmd.m_targetReachedFunc = [&](){ undoPhase(); };
        animCmd.Undo();
    }
    else
        unblockUI();
    UpdateUI();
}

void Command::Do()
{
    m_uiPhase = 0;
    doPhase();
}

void Command::Undo()
{
    m_uiPhase = Cast2Int(m_phases.size());
    undoPhase();
}

void Command::AddPhase(unique_ptr<Command> upCmd)
{
    m_phases.push_back(move(upCmd));
}

void Command::blockUI() const
{ 
    m_pWin->SendCommand2Application(IDM_BLOCK_UI, true);  
};

void Command::unblockUI() const
{ 
    NextScriptCommand();   // script continuation for async commands
    m_pWin->PostCommand2Application(IDM_BLOCK_UI, false);
};

void Command::NextScriptCommand()
{
    if (ScriptStack::IsScriptActive() && !ScriptStack::SingleStepMode())
        m_pWin->PostCommand2Application(IDM_NEXT_SCRIPT_CMD, 0); 
}
