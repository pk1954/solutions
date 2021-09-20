// NNetCommand.h
//
// NNetWindows

#pragma once

#include "win32_animationCmd.h"

class NNetModelWriterInterface;

class NNetCommand : public AnimationCmd
{
public:
    NNetCommand() {}

    static void Initialize(NNetModelWriterInterface * const pNMWI)
    {
        m_pNMWI = pNMWI;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};