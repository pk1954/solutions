// NNetCommandStack.ixx
//
// NNetCommands

export module NNetCommands:NNetCommandStack;

import CommandStack;
import NNetModel;

export class NNetCommandStack : public CommandStack
{
public:
    virtual ~NNetCommandStack() = default;

    static void SetModelInterface(NNetModelWriterInterface* const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

    void Check() final
    {
        if (m_pNMWI)
            m_pNMWI->CheckModel();
    }

private:

    inline static NNetModelWriterInterface* m_pNMWI { nullptr };

};