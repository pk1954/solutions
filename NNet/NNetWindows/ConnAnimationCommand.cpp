// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "win32_rootWindow.h"
#include "win32_Commands.h"
#include "AlignDirectionCommand.h"
#include "AlignPositionsCommand.h"
#include "PackShapesCommand.h"
#include "CreateConnectorCommand.h"
#include "CommandStack.h"
#include "ShapeIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "ConnAnimationCommand.h"

using std::make_unique;

ConnAnimationCommand::ConnAnimationCommand
( 
    unique_ptr<ShapePtrList<ConnNeuron>> upShapesAnimated,
    RootWindow                         * pWin,
    WinCommands                        & cmds,
    int                          const   iStep,
    bool                         const   bBackwards
)
  : m_upShapesAnimated(move(upShapesAnimated)),
    m_pWin(pWin),
    m_winCommands(cmds),
    m_iStep(iStep),
    m_bBackwards(bBackwards)
{}

void ConnAnimationCommand::initialize( NNetModelWriterInterface& nmwi )
{
    m_upConnAnimation = make_unique<Animation<MicroMeterPointVector>>
    (
        [&](bool const bTargetReached) 
        { 
            m_callable.Call_UI_thread
            (
                [&]()
                {
                    m_winCommands.Update(this);
                    m_pWin->Notify(false);
                }
            );
            if (TargetReached())
            {
                if (Forwards())
                {
                    if (m_iStep)
                    {
                        m_callable.Call_UI_thread
                        (
                            [&]()
                            {
                                switch (m_iStep)
                                {
                                case 1:	AlignPositions(m_pWin, 2, false); break;
                                case 2:	AlignDirection(m_pWin, 3, true);	break;
                                case 3:	PackShapes    (m_pWin, 4, true);	break;
                                case 4:	CreateConnector();      		    break;
                                default: break;
                                }
                            }
                        );
                    }
                }
                else
                {
                    if (m_bBackwards)
                        m_pWin->PostCommand(IDM_UNDO);
                }
            }
        }
    );
    prepareData(nmwi);
    m_umPntVectorStart  = MicroMeterPointVector(*m_upShapesAnimated.get());
    m_umPntVectorTarget = MicroMeterPointVector(*m_upShapesAnimated.get());
    DefineTarget();
    m_upConnAnimation->SetNrOfSteps( calcNrOfSteps(m_umPntVectorStart, m_umPntVectorTarget) );
    m_bInitialized = true;
}

void ConnAnimationCommand::Do( NNetModelWriterInterface& nmwi )
{
    if ( ! m_bInitialized )
        initialize(nmwi);
    m_bForwards = true;
    m_upConnAnimation->Start(m_umPntVectorStart, m_umPntVectorTarget);
}

void ConnAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_bForwards = false;
    m_upConnAnimation->Start(m_umPntVectorTarget, m_umPntVectorStart);
}

void ConnAnimationCommand::AlignPositions(RootWindow * pWin, int const iMsg, bool const bBackwards)
{ 
    m_winCommands.GetCommandStack().PushCommand( make_unique<AlignPositionsCommand>(pWin, m_winCommands, iMsg, bBackwards) );
}

void ConnAnimationCommand::AlignDirection(RootWindow * pWin, int const iMsg, bool const bBackwards)
{ 
    m_winCommands.GetCommandStack().PushCommand( make_unique<AlignDirectionCommand>(pWin, m_winCommands, iMsg, bBackwards) );
}

void ConnAnimationCommand::PackShapes(RootWindow * pWin, int const iMsg, bool const bBackwards)
{
    m_winCommands.GetCommandStack().PushCommand( make_unique<PackShapesCommand>(pWin, m_winCommands, iMsg, bBackwards) );
}

void ConnAnimationCommand::CreateConnector()
{
    m_winCommands.GetCommandStack().PushCommand( make_unique<CreateConnectorCommand>(move(CreateShapeList())) );
}

unique_ptr<ShapePtrList<ConnNeuron>> ConnAnimationCommand::CreateShapeList()
{
    unique_ptr<ShapePtrList<ConnNeuron>> upShapesAnimated;
    ShapeType          const shapeType { m_winCommands.GetNMWI().GetUPShapes().DetermineShapeType() };
    if ( ! shapeType.IsUndefinedType() )
        return make_unique<ShapePtrList<ConnNeuron>>(m_winCommands.GetNMWI().GetUPShapes().GetAllSelected<ConnNeuron>(shapeType));
    return nullptr;
}

unsigned int const ConnAnimationCommand::calcNrOfSteps
(
    MicroMeterPointVector const & umPntVectorStart,
    MicroMeterPointVector const & umPntVectorTarget
) const
{
    MicroMeterPointVector const umPntVectorDiff { umPntVectorTarget - umPntVectorStart };
    Radian                const radDiffMax      { umPntVectorDiff.FindMaxRadian() };
    Radian                const radPerStep      { Degrees2Radian(6.0_Degrees) };
    float                 const fStepsFromRot   { radDiffMax / radPerStep };

    MicroMeter            const umDiffMax       { umPntVectorDiff.FindMaxPos() };
    MicroMeter            const umPerStep       { NEURON_RADIUS / 5.0f };
    float                 const fStepsFromMove  { umDiffMax / umPerStep };

    float                 const fSteps          { max(fStepsFromRot, fStepsFromMove) };
    unsigned int          const uiSteps         { Cast2UnsignedInt(fSteps) + 1 };
    return uiSteps;
}

bool const ConnAnimationCommand::prepareData(NNetModelWriterInterface& nmwi)
{
    m_line.Set2Null();
    m_line = m_upShapesAnimated->CalcMaxDistLine();
    if ( m_line.IsZero() )
        return false;

    m_upShapesAnimated->SortAccToDistFromLine( m_line.OrthoLine() );
    return true;
}
