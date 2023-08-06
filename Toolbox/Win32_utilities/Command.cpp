// Command.cpp
//
// Win32_utilities

module;

#include <cassert>
#include <bit>
#include <memory>
#include <Windows.h>

module Command;

import Win32_Util_Resource;
import WinManager;
import SaveCast;
import RootWindow;
import ScriptStack;

using std::bit_cast;
using std::unique_ptr;

void Command::UpdateUI()
{
    WinManager::GetBaseWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
};

void Command::CallUI(bool const bTargetReached)  // runs in animation thread
{
    WinManager::PostMessage
    (
        RootWinId(IDM_MAIN_WINDOW),
        WM_APP_UI_CALL,            // calls DoCall from UI thread
        static_cast<WPARAM>(bTargetReached),
        bit_cast<LPARAM>(this)
    );
}

void Command::DoCall(WPARAM const wParam, LPARAM const lParam) // runs in UI thread
{
    Command* const pAnimCmd { bit_cast<Command* const>(lParam) };
    bool      const bTargetReached { static_cast<bool const>(wParam) };
    pAnimCmd->UpdateUI();
    if (bTargetReached && pAnimCmd->m_targetReachedFunc)
        (pAnimCmd->m_targetReachedFunc)();
};

void Command::doPhase() // runs in UI thread
{
    if (!m_phases.empty())
    {
        if (m_uiPhase == 0)
            blockUI();
        if (m_uiPhase < m_phases.size())
        {
            Command* const pAnimCmd { m_phases[m_uiPhase++].get() };
            pAnimCmd->m_targetReachedFunc = [&]() { doPhase(); };
            pAnimCmd->Do();
        }
        else
            unblockUI();
    }
    UpdateUI();
}

void Command::undoPhase() // runs in UI thread
{
    if (m_uiPhase >= m_phases.size())
        blockUI();
    if (m_uiPhase > 0)
    {
        Command& animCmd { *m_phases[--m_uiPhase] };
        animCmd.m_targetReachedFunc = [&]() { undoPhase(); };
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
    m_uiPhase = Cast2UnsignedInt(m_phases.size());
    undoPhase();
}

void Command::AddPhase(unique_ptr<Command> upCmd)
{
    m_phases.push_back(move(upCmd));
}

void Command::blockUI() const
{
    WinManager::PostCommand2App(IDM_BLOCK_UI, true);
};

void Command::unblockUI() const
{
    NextScriptCommand();   // script continuation for async commands
    WinManager::PostCommand2App(IDM_BLOCK_UI, false);
};

void Command::NextScriptCommand()
{
    if (ScriptStack::IsScriptActive() && !ScriptStack::SingleStepMode())
        WinManager::PostCommand2App(IDM_NEXT_SCRIPT_CMD, 0);
}
