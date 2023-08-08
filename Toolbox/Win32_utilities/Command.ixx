// Command.ixx
//
// Win32_utilities

module;

#include <memory>
#include <string>
#include <Windows.h>

export module Command;

import StdStackCommand;
import SoundInterface;

using std::unique_ptr;

export class Command: public StdStackCommand
{
public:

    void UpdateUI()                override;
    void NextScriptCommand() const final;

    void CallUI(bool const) final; // called by Animation

    static void DoCall(WPARAM const, LPARAM const); // called by m_pWin

private:

    void BlockUI()   const final;
    void UnblockUI() const final;
};
