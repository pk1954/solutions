// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Command.h"
#include "ShapePtrList.h"
#include "MicroMeterPointVector.h"
#include "win32_animation.h"

class NNetModelWriterInterface;
class NNetModelCommands;
class RootWindow;

class ConnAnimationCommand : public Command
{
public:
    ConnAnimationCommand
    (
        unique_ptr<ShapePtrList<ConnNeuron>>         const,
        function<void(ConnAnimationCommand const *)> const
    );
    virtual ~ConnAnimationCommand() {};

    virtual void Do  (NNetModelWriterInterface&);
    virtual void Undo(NNetModelWriterInterface&);

    virtual void DefineTarget() = 0;

    MicroMeterPointVector    const   GetActual()         { return m_upConnAnimation->GetActual(); }
    ShapePtrList<ConnNeuron> const & GetAnimatedShapes() { return * m_upShapesAnimated.get(); }

    bool const TargetReached() const { return m_upConnAnimation->TargetReached(); }
    bool const Forwards     () const { return m_bForwards; }

protected:
    MicroMeterPointVector                m_umPntVectorTarget;
    MicroMeterLine                       m_line { MicroMeterLine::NULL_VAL() };
    unique_ptr<ShapePtrList<ConnNeuron>> m_upShapesAnimated;

private:
    bool                                         m_bInitialized { false };
    bool                                         m_bForwards    { false };
    MicroMeterPointVector                        m_umPntVectorStart;
    unique_ptr<Animation<MicroMeterPointVector>> m_upConnAnimation;
    function<void(ConnAnimationCommand const *)> const m_func;

    void               initialize(NNetModelWriterInterface&);

    bool               const prepareData(NNetModelWriterInterface&);
    unsigned int       const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
};