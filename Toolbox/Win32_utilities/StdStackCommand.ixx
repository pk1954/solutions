// StdStackCommand.ixx
//
// Win32_utilities

module;

#include <memory>
#include <string>

export module StdStackCommand;

export import Command;
export import CommandStack;
export import SoundInterface;

export using std::unique_ptr;
export using std::wstring;

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
        m_pSound = pSound;
    }

    static void PushCommand(unique_ptr<Command> cmd) { m_pStack->PushStackCommand(move(cmd)); }
    static bool UndoCommand()                        { return m_pStack->UndoStackCommand(); }
    static bool RedoCommand()                        { return m_pStack->RedoStackCommand(); }
    static void ClearStack ()                        { m_pStack->Clear(); }
    static void PlaySound(wstring const& sound)      { m_pSound->Play(sound); }
    static void PlayWarningSound()                   { m_pSound->Warning(); }

private:

    inline static CommandStack* m_pStack { nullptr };
    inline static Sound       * m_pSound { nullptr };
};
