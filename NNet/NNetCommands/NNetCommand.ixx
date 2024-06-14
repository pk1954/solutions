// NNetCommand.ixx
//
// NNetCommands

module;

#include <string>
#include <memory>
#include <iostream>

export module NNetCommands:NNetCommand;

export import IoUtil;
export import SoundInterface;
export import Types;
export import WinCommand;
export import NNetModel;
export import NNetWrapperHelpers;

export using std::make_unique;
export using std::unique_ptr;

export class NNetCommand : public WinCommand
{
public:
    ~NNetCommand() override = default;

    static void SetModelInterface(NNetModelWriterInterface* const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    virtual NobId GetAffectedNob() const
    {
        return NO_NOB;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};
