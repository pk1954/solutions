// StdStackCommand.ixx
//
// Toolbox\Commands

module;

#include <memory>
#include <string>
#include <iostream>

export module StdStackCommand;

import BaseCommand;
import CommandStack;
import SoundInterface;

export import Wrapper;
export import Symtab;
export import Script;

export using std::unique_ptr;
export using std::wstring;
export using std::endl;

export class StdStackCommand : public BaseCommand
{
public:

    static void Initialize
    (
        CommandStack* const pStack,
        Sound       * const pSound
    )
    {
        m_pStack = pStack; 
        BaseCommand::Initialize(pSound);
    }

    static void PushCommand(unique_ptr<StdStackCommand> cmd) { m_pStack->PushStackCommand(move(cmd)); }
    static bool UndoCommand()                                { return m_pStack->UndoStackCommand(); }
    static bool RedoCommand()                                { return m_pStack->RedoStackCommand(); }
    static void ClearStack ()                                { m_pStack->Clear(); }

private:

    inline static CommandStack* m_pStack { nullptr };
};
