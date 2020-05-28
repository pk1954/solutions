// NNetModelWriterInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "ShapeId.h"
#include "NNetModel.h"

using std::wostream;

class Param;
class NNetModel;
class ActionTimer;
class SlowMotionRatio;
class NNetModelStorage;
class PixelCoordsFp;

class NNetModelWriterInterface
{
public:
	void Initialize( std::wostream * );

	void Start( NNetModel * const, Param * const );
	void Stop(); 

	void ActionCommand       ( int const, ShapeId const, MicroMeterPoint const & );
	void Connect             ( ShapeId const, ShapeId const );
	void Convert2InputNeuron ( ShapeId const );
	void Convert2Neuron      ( ShapeId const );
	void ToggleStopOnTrigger ( ShapeId const );
	void CopySelection       ( );
	void DeleteSelection     ( );
	void Disconnect          ( ShapeId const );
	void MarkSelection       ( tBoolOp const );
	void MoveSelection       ( MicroMeterPoint const & );
	void MoveShape           ( ShapeId const, MicroMeterPoint const & );
	void RemoveBeepers       ( );
	void RemoveShape         ( ShapeId const );
	void ResetModel          ( );
	void ResetTimer          ( );
	void SelectAll           ( tBoolOp const );
	void SelectAllBeepers    ( );
	void SelectShape         ( ShapeId const, tBoolOp const );
	void SelectShapesInRect  ( MicroMeterRect const & );
	void SelectSubtree       ( ShapeId const, tBoolOp const );
	void SetParameter        ( tParameter const, float const );
	void SetPulseRate        ( ShapeId    const, fHertz const );
	void SetTriggerSound     ( ShapeId const, bool const, Hertz const, MilliSecs const );

	wchar_t const * GetActionCommandName    ( int const ) const;
	int     const   GetActionCommandFromName( wchar_t const * const ) const;

private:

	bool       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

	bool        m_bTrace       { true };
	wostream  * m_pTraceStream { nullptr };
	NNetModel * m_pModel       { nullptr };
	Param     * m_pParam       { nullptr };
}; 
