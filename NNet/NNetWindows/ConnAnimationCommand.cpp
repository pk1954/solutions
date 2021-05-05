// ConnAnimationCommand.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "CalcOrthoVector.h"
#include "win32_rootWindow.h"
#include "win32_Commands.h"
#include "CommandStack.h"
#include "ShapeIdList.h"
#include "NNetModelCommands.h"
#include "NNetModelWriterInterface.h"
#include "ConnAnimationCommand.h"

using std::make_unique;

ConnAnimationCommand::ConnAnimationCommand
( 
    RootWindow  & win,
    WinCommands & cmds
)
  : m_win(win),
    m_NMWI(cmds.GetNMWI()),
    m_callable(win.GetWindowHandle())
{
    m_pModelShapes = &m_NMWI.GetUPShapes();
    ShapeType const shapeType { determineShapeType() };
    if ( shapeType.IsUndefinedType() )
        return;
    
    m_shapesAnimated = ShapePtrList<ConnNeuron>(m_pModelShapes->GetAllSelected<ConnNeuron>(shapeType));

    MicroMeterLine line { MicroMeterLine::NULL_VAL() };
    line = m_shapesAnimated.CalcMaxDistLine();
    if ( line.IsZero() )
        return;

    m_shapesAnimated.SortAccToDistFromLine( line.OrthoLine() );
    m_upConnector  = make_unique<Connector>( m_shapesAnimated );
    
    m_umPntVectorOriginal = MicroMeterPointVector( m_shapesAnimated );

    m_umPntVectorTarget1  = m_umPntVectorOriginal;
    m_umPntVectorTarget1.Align(line);

    m_umPntVectorTarget2  = m_umPntVectorTarget1;
    m_umPntVectorTarget2.SetDir(Vector2Radian(CalcOrthoVector(line, m_shapesAnimated)));

    m_umPntVectorTarget3  = m_umPntVectorTarget2;
    m_umPntVectorTarget3.Pack(NEURON_RADIUS * 1.8f);
}

ShapeType const ConnAnimationCommand::determineShapeType() const
{
    unsigned int uiNrOfConnectors { m_pModelShapes->CountInSelection( ShapeType::Value::connector ) };

    if ( uiNrOfConnectors > 0 )
        return ShapeType::Value::undefined;

    unsigned int uiNrOfInputNeurons  { m_pModelShapes->CountInSelection( ShapeType::Value::inputNeuron  ) };
    unsigned int uiNrOfOutputNeurons { m_pModelShapes->CountInSelection( ShapeType::Value::outputNeuron ) };

    if ((uiNrOfInputNeurons == 0) && (uiNrOfOutputNeurons == 0))
        return ShapeType::Value::undefined;

    return (uiNrOfInputNeurons > uiNrOfOutputNeurons) 
        ? ShapeType::Value::inputNeuron 
        : ShapeType::Value::outputNeuron;
}

void ConnAnimationCommand::updateUI()  // runs in animation thread
{
    m_NMWI.SetConnNeurons
    (
        m_connAnimation.GetActual(), 
        m_shapesAnimated
    );
    m_win.Notify(false);
}

void ConnAnimationCommand::nextAnimationPhase() // runs in UI thread
{
    MicroMeterPointVector umPntVectorStart { MicroMeterPointVector( m_shapesAnimated ) };
    MicroMeterPointVector umPntVectorTarget;

    if (m_mode == Mode::mode_do)
    {
        switch (m_iPhase++)
        {
        case 1:  blockUI();
                 umPntVectorTarget = m_umPntVectorTarget1; break;
        case 2:	 umPntVectorTarget = m_umPntVectorTarget2; break;
        case 3:  umPntVectorTarget = m_umPntVectorTarget3; break;
        case 4:	 m_upConnector->SetParentPointers();
                 m_pModelShapes->Push( move(m_upConnector) );
                 unblockUI();
                 [[fallthrough]]; 
        default: return;        // do not start animation
        }
    }
    else
    {
        switch (m_iPhase--)
        {
        case 4:	 blockUI();
                 m_upConnector = m_pModelShapes->Pop<Connector>();
                 m_upConnector->ClearParentPointers();
                 [[fallthrough]]; 
        case 3:	 umPntVectorTarget = m_umPntVectorTarget2;  break;
        case 2:	 umPntVectorTarget = m_umPntVectorTarget1;  break;
        case 1:	 umPntVectorTarget = m_umPntVectorOriginal; break;
        case 0:  unblockUI();
                 [[fallthrough]]; 
        default: return;                // do not start animation
        }
    }
    m_connAnimation.SetNrOfSteps( calcNrOfSteps(umPntVectorStart, umPntVectorTarget) );
    m_connAnimation.Start(umPntVectorStart, umPntVectorTarget);
}

void ConnAnimationCommand::Do( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_do;
    m_iPhase = 1;
    nextAnimationPhase();
}

void ConnAnimationCommand::Undo( NNetModelWriterInterface& nmwi )
{
    m_mode = Mode::mode_undo;
    m_iPhase = 4;
    nextAnimationPhase();
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
