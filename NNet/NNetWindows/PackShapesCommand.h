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
        unique_ptr<ShapePtrList<ConnNeuron>>                upShapesAnimated,
        function<void(ConnAnimationCommand const *)> const & func
    )
        : ConnAnimationCommand(move(upShapesAnimated), func)
    {}

private:
    virtual void DefineTarget()
    {
        m_umPntVectorTarget.Pack(NEURON_RADIUS * 1.8f);
    }
};