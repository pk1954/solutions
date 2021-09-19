// NNetCommand.h
//
// NNetWindows

#pragma once

#include "AnimationCmd.h"

class NNetModelWriterInterface;

class NNetCommand : public AnimationCmd
{
public:
    NNetCommand() {}

    static void Initialize
    (
        NNetModelWriterInterface * const pNMWI,
        RootWindow               * const pWin
    )
    {
        AnimationCmd::Initialize(pWin);
        m_pNMWI = pNMWI;
    }

protected:

    inline static NNetModelWriterInterface * m_pNMWI { nullptr };
};