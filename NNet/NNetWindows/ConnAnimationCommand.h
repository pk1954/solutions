// ConnAnimationCommand.h
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "Command.h"
#include "ShapePtrList.h"
#include "MicroMeterPointVector.h"
#include "win32_callable.h"
#include "win32_animation.h"

class NNetModelWriterInterface;
class NNetModelCommands;
class ConnAnimationCommand;
class WinCommands;
class RootWindow;

class ConnAnimationCommand : public Command
{
public:
    ConnAnimationCommand
    (
        RootWindow  &,
        WinCommands &,
        int  const,
        bool const
    );
    virtual ~ConnAnimationCommand() {};

    virtual void Do  (NNetModelWriterInterface&);
    virtual void Undo(NNetModelWriterInterface&);

    virtual void DefineTarget() = 0;

    void AlignDirection  (RootWindow &, int const, bool const);
    void AlignPositions  (RootWindow &, int const, bool const);
    void PackShapes      (RootWindow &, int const, bool const);
    void CreateConnector();

    MicroMeterPointVector    const   GetActual()         { return m_upConnAnimation->GetActual(); }
    ShapePtrList<ConnNeuron> const & GetAnimatedShapes() { return * m_upShapesAnimated.get(); }

    bool const TargetReached() const { return m_upConnAnimation->TargetReached(); }
    bool const Forwards     () const { return m_bForwards; }

protected:
    MicroMeterPointVector                m_umPntVectorTarget;
    MicroMeterLine                       m_line { MicroMeterLine::NULL_VAL() };
    unique_ptr<ShapePtrList<ConnNeuron>> m_upShapesAnimated {};

    unique_ptr<ShapePtrList<ConnNeuron>> CreateShapeList();

private:
    bool                                         m_bInitialized { false };
    bool                                         m_bForwards    { false };
    MicroMeterPointVector                        m_umPntVectorStart;
    unique_ptr<Animation<MicroMeterPointVector>> m_upConnAnimation;

    WinCommands & m_winCommands;
    RootWindow  & m_win;
    int     const m_iStep;
    bool    const m_bBackwards;
    Callable      m_callable;

    void               initialize(NNetModelWriterInterface&);
    bool         const prepareData(NNetModelWriterInterface&);
    unsigned int const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
};