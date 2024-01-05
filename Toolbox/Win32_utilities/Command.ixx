// Command.ixx
//
// Toolbox\Win32_utilities

module;

#include <memory>
#include <bit>
#include <Windows.h>

export module Command;

import StdStackCommand;
import Win32_Util_Resource;
import WinManager;
import ScriptStack;

using std::bit_cast;
using std::unique_ptr;

export class Command: public StdStackCommand
{
public:
    void UpdateUI() override
    {
        WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
    };

    void NextScriptCommand() const final
    {
        if (ScriptStack::IsScriptActive() && !ScriptStack::SingleStepMode())
            WinManager::PostCommand2App(IDM_NEXT_SCRIPT_CMD, 0);
    }

    void CallUI(bool const bTargetReached) final // runs in Animation thread
    {
        WinManager::PostMessage2MainWin
        (
            WM_APP_UI_CALL,            // calls DoCall from UI thread
            static_cast<WPARAM>(bTargetReached),
            bit_cast<LPARAM>(this)
        );
    }

    static void DoCall(WPARAM const wParam, LPARAM const lParam)  // called by m_pWin
    {
        Command* const pAnimCmd       { bit_cast<Command* const>(lParam) };
        bool     const bTargetReached { static_cast<bool const>(wParam) };
        pAnimCmd->UpdateUI();
        if (bTargetReached)
            pAnimCmd->TargetReached();
    };
};
