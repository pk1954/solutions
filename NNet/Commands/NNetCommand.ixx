// NNetCommand.ixx
//
// Commands

module;

#include <string>
#include <memory>
#include <iostream>

export module NNetCommand;

export import SoundInterface;
export import Types;
export import Symtab;
export import Script;
export import Commands;
export import NNetModel;
export import NNetWrapperHelpers;

export using std::endl;
export using std::wstring;
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

    static void SetSound(Sound * pSound)
    {
        m_pSound = pSound;
    }

    virtual NobId GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI  { nullptr };
    inline static Sound *                    m_pSound { nullptr };
};
