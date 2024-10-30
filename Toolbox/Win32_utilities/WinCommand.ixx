// WinCommand.ixx
//
// Toolbox\Win32_utilities

export module WinCommand;

import std;
import Commands;
import RunTime;
import Win32_Util_Resource;
import WinBasics;
import WinManager;

using std::bit_cast;
using std::unique_ptr;

export using std::endl;

export class WinCommand: public BaseCommand
{
public:
    static void Initialize (CommandStack* const pStack)  { m_pStack = pStack; }
    static void PushCommand(unique_ptr<BaseCommand> cmd) { m_pStack->PushStackCommand(move(cmd)); }
    static void ClearStack ()                            { m_pStack->Clear(); }

    void NextScriptCommand() const final
    {
        if (ScriptStack::IsScriptActive() && !ScriptStack::SingleStepMode())
            WinManager::PostCommand2App(IDM_NEXT_SCRIPT_CMD, 0);
    }

    static void AnimationUpdate(WPARAM const wParam, LPARAM const lParam)  // called by m_pWin
    {
        WinCommand* const pAnimCmd { bit_cast<WinCommand* const>(lParam) };
        pAnimCmd->UpdateUI();
    };

private:

    inline static CommandStack* m_pStack { nullptr };
};
