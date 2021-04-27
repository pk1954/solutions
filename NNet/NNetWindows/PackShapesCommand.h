// PackShapesCommand.h
//
// NNetWindows

#pragma once

#include "ConnAnimationCommand.h"

class RootWindow;

class PackShapesCommand : public ConnAnimationCommand
{
public:
    PackShapesCommand
    (
        unique_ptr<ShapePtrList<ConnNeuron>> upShapesAnimated,
        RootWindow                   const * pWin,
        int                          const   iMsg,
        bool                         const   bBackwards
    )
        : ConnAnimationCommand(move(upShapesAnimated), pWin, iMsg, bBackwards)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.Pack(NEURON_RADIUS * 1.8f);
    }
};