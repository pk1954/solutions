// NNetCommand.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module NNetCommands:NNetCommand;

export import IoUtil;
export import SoundInterface;
export import Types;
export import Command;
export import NNetModel;
export import NNetWrapperHelpers;

export using std::make_unique;
export using std::unique_ptr;

export class NNetCommand : public Command
{
public:
    ~NNetCommand() override = default;

    static void SetModelInterface(NNetModelWriterInterface* const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    static void Initialize(Sound * pSound)
    {
        m_pSound = pSound;
    }

    virtual NobId GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    static void PlaySound(wstring const& sound)
    {
        m_pSound->Play(sound);
    }

    static void PlayWarningSound()
    {
        m_pSound->Warning();
    }

    inline static NNetModelWriterInterface * m_pNMWI  { nullptr };

private:

    inline static Sound* m_pSound { nullptr };
};
