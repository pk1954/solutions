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
import RootWindow;
import ScriptStack;

using std::bit_cast;
using std::unique_ptr;

void Command::UpdateUI()
{
    WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
};

void Command::CallUI(bool const bTargetReached)  // runs in animation thread
{
    WinManager::PostMessage2MainWin
    (
        WM_APP_UI_CALL,            // calls DoCall from UI thread
        static_cast<WPARAM>(bTargetReached),
        bit_cast<LPARAM>(this)
    );
}

void Command::DoCall(WPARAM const wParam, LPARAM const lParam) // runs in UI thread
{
    Command* const pAnimCmd       { bit_cast<Command* const>(lParam) };
    bool     const bTargetReached { static_cast<bool const>(wParam) };
    pAnimCmd->UpdateUI();
    if (bTargetReached)
        pAnimCmd->TargetReached();
};

void Command::BlockUI() const
{
    WinManager::PostCommand2App(IDM_BLOCK_UI, true);
};

void Command::UnblockUI() const
{
    NextScriptCommand();   // script continuation for async commands
    WinManager::PostCommand2App(IDM_BLOCK_UI, false);
};

void Command::NextScriptCommand() const
{
    if (ScriptStack::IsScriptActive() && !ScriptStack::SingleStepMode())
        WinManager::PostCommand2App(IDM_NEXT_SCRIPT_CMD, 0);
}
