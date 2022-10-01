// NNetCommand.ixx
//
// Commands

export module NNetCommand;

import Command;
import NNetModel;

export class NNetCommand : public Command
{
public:
    ~NNetCommand() override = default;

    static void SetModelInterface(NNetModelWriterInterface * const pNMWI)
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