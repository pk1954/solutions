// Command.ixx
//
// Win32_utilities

module;

#include <memory>
#include <string>
#include <Windows.h>

export module Command;

import BaseCommand;
import SoundInterface;

using std::unique_ptr;

export class Command: public BaseCommand
{
public:

    virtual ~Command() = default;
    
    void UpdateUI() override;

    virtual bool CombineCommands(Command const& src) { return false; };
    virtual bool IsAsyncCommand() { return false; };

    void CallUI(bool const); // called by Animation

    static void DoCall(WPARAM const, LPARAM const); // called by m_pWin
    static void NextScriptCommand();

private:

    void BlockUI()   const final;
    void UnblockUI() const final;
};
