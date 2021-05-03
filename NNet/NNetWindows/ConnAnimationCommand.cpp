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
    m_winCommands(cmds),
    m_callable(win.GetWindowHandle())
{
    m_pModelShapes = & m_winCommands.GetNMWI().GetUPShapes();
    ShapeType const shapeType { determineShapeType() };
    if ( shapeType.IsUndefinedType() )
        return;
    
    m_shapesAnimated = ShapePtrList<ConnNeuron>(m_pModelShapes->GetAllSelected<ConnNeuron>(shapeType));

    m_line.Set2Null();
    m_line = m_shapesAnimated.CalcMaxDistLine();
    if ( m_line.IsZero() )
        return;

    m_shapesAnimated.SortAccToDistFromLine( m_line.OrthoLine() );
    m_upConnector  = make_unique<Connector>( m_shapesAnimated );
    m_radianTarget = Vector2Radian(CalcOrthoVector(m_line, m_shapesAnimated));
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
    m_winCommands.GetNMWI().SetConnNeurons
    (
        m_connAnimation.GetActual(), 
        m_shapesAnimated
    );
    m_win.Notify(false);
}

void ConnAnimationCommand::nextAnimationPhase() // runs in UI thread
{
    MicroMeterPointVector umPntVectorStart  = MicroMeterPointVector( m_shapesAnimated );
    MicroMeterPointVector umPntVectorTarget = MicroMeterPointVector( m_shapesAnimated );

    int iPhase { m_iPhase };

    if (m_mode == Mode::mode_do)
    {
        wcout << L"mode_do phase " << iPhase << endl;
        ++m_iPhase;
        switch (iPhase)
        {
        case 1:  blockUI();
                 m_umPntVectorOriginal = umPntVectorTarget;
                 umPntVectorTarget.Align(m_line);
                 m_umPntVectorTarget1 = umPntVectorTarget;
                 break;
        case 2:	 umPntVectorTarget.SetDir(m_radianTarget);    
                 m_umPntVectorTarget2 = umPntVectorTarget;     
                 break;
        case 3:  umPntVectorTarget.Pack(NEURON_RADIUS * 1.8f);
                 break;
        case 4:	 m_upConnector->SetParentPointers();
                 m_pModelShapes->Push( move(m_upConnector) );
                 unblockUI();
                 [[fallthrough]]; 
        default: return;        // do not start animation
        }
    }
    else
    {
        wcout << L"mode_undo phase " << iPhase << endl;
        --m_iPhase;
        switch (iPhase)
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
