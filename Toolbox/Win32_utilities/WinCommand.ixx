// WinCommand.ixx
//
// Toolbox\Win32_utilities

module;

#include <iostream>
#include <memory>
#include <bit>
#include <Windows.h>

export module WinCommand;

import Win32_Util_Resource;
import WinManager;
import ScriptStack;
import Commands;

using std::bit_cast;
using std::unique_ptr;

export using std::endl;

export class WinCommand: public BaseCommand
{
public:
    static void Initialize (CommandStack* const pStack)  { m_pStack = pStack; }
    static void PushCommand(unique_ptr<BaseCommand> cmd) { m_pStack->PushStackCommand(move(cmd)); }
    static void ClearStack ()                            { m_pStack->Clear(); }

    void UpdateUI() override
    {
        WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
    };

    void NextScriptCommand() const final
    {
        if (ScriptStack::IsScriptActive() && !ScriptStack::SingleStepMode())
            WinManager::PostCommand2App(IDM_NEXT_SCRIPT_CMD, 0);
    }

    void CallUI() final // runs in Animation thread
    {
        WinManager::PostMessage2MainWin(WM_APP_UI_CALL, 0, bit_cast<LPARAM>(this)); // calls DoCall from UI thread
    }

    static void DoCall(WPARAM const wParam, LPARAM const lParam)  // called by m_pWin
    {
        WinCommand* const pAnimCmd { bit_cast<WinCommand* const>(lParam) };
        pAnimCmd->UpdateUI();
    };

private:

    inline static CommandStack* m_pStack { nullptr };
};
