// StdStackCommand.ixx
//
// Win32_utilities

module;

#include <memory>

export module StdStackCommand;

import Command;
import CommandStack;
import SoundInterface;

export using std::unique_ptr;

export class StdStackCommand : public Command
{
public:
    ~StdStackCommand() override = default;

    static void Initialize
    (
        CommandStack* const pStack,
        Sound       * const pSound
    )
    {
        m_pStack = pStack; 
        Command::Initialize(pSound);
    }

    static void PushCommand(unique_ptr<Command> cmd) { m_pStack->PushStackCommand(move(cmd)); }
    static bool UndoCommand()                        { return m_pStack->UndoStackCommand(); }
    static bool RedoCommand()                        { return m_pStack->RedoStackCommand(); }
    static void ClearStack ()                        { m_pStack->Clear(); }

private:

    inline static CommandStack* m_pStack { nullptr };
};
