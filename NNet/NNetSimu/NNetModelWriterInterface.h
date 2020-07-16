// NNetModelWriterInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "NNetModel.h"
#include "Command.h"

using std::wostream;

class NNetModel;
class ActionTimer;
class SlowMotionRatio;
class NNetModelStorage;
class PixelCoordsFp;

class NNetModelWriterInterface
{
public:
	void Initialize( std::wostream * );

	void Start( NNetModel * const );
	void Stop(); 

/* !! */ void Action              ( int const, ShapeId const, MicroMeterPoint const & );
/* ok */ void Connect             ( ShapeId const, ShapeId const );
         void Convert2InputNeuron ( ShapeId const );
         void Convert2Neuron      ( ShapeId const );
         void CopySelection       ( );
         void DeleteSelection     ( );
         void Disconnect          ( ShapeId const );
         void MarkSelection       ( tBoolOp const );
/* ok */ void MoveSelection       ( MicroMeterPoint const & );
/* ok */ void MoveShape           ( ShapeId const, MicroMeterPoint const & );
         void ClearBeepers        ( );
/* ok */ void RemoveShape         ( ShapeId const );
         void ResetModel          ( );
/* ok */ void ResetTimer          ( );
         void SelectAll           ( tBoolOp const );
         void SelectAllBeepers    ( );
/* ok */ void SelectShape         ( ShapeId const, tBoolOp const );
         void SelectShapesInRect  ( MicroMeterRect const & );
         void SelectSubtree       ( ShapeId const, tBoolOp const );
/* ok */ void SetParameter        ( tParameter const, float const );
/* ok */ void SetPulseRate        ( ShapeId    const, fHertz const );
         void SetTriggerSound     ( ShapeId const, bool const, Hertz const, MilliSecs const );
/* ok */ void ToggleStopOnTrigger ( ShapeId const );

	void UndoCommand();
	void RedoCommand();

	wchar_t const * GetActionCommandName    ( int const ) const;
	int     const   GetActionCommandFromName( wchar_t const * const ) const;

private:

	bool       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

	bool        m_bTrace       { true };
	wostream  * m_pTraceStream { nullptr };
	NNetModel * m_pModel       { nullptr };

	CommandStack m_CmdStack {};
}; 
