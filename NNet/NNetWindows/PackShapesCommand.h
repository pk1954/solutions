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
        RootWindow                 * const   pWin, 
        function<void()>             const & finFunc
    )
        : ConnAnimationCommand(move(upShapesAnimated), pWin, finFunc)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.Pack(NEURON_RADIUS * 1.8f);
    }
};