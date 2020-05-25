// win32_WorkThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "ShapeId.h"
#include "NNetModel.h"
#include "win32_WorkThread.h"

using std::wostream;

class Param;
class ActionTimer;
class EventInterface;
class SlowMotionRatio;
class NNetModelStorage;
class PixelCoordsFp;

class WorkThreadInterface
{
public:
	WorkThreadInterface( );
    ~WorkThreadInterface( );

	void Initialize( std::wostream * );

	void Start
    ( 
		HWND               const,
		ActionTimer      * const,
		EventInterface   * const,
		SlowMotionRatio  * const,
		NNetModel        * const,
		Param            * const,
		NNetModelStorage * const,
		bool               const
	);
	void Stop(); 

	void PostActionCommand       ( int const, ShapeId const, MicroMeterPoint const & );
	void PostConnect             ( ShapeId const, ShapeId const );
	void PostConvert2InputNeuron ( ShapeId const );
	void PostConvert2Neuron      ( ShapeId const );
	void PostToggleStopOnTrigger ( ShapeId const );
	void PostCopySelection       ( );
	void PostDeleteSelection     ( );
	void PostDisconnect          ( ShapeId const );
	void PostMarkSelection       ( tBoolOp const );
	void PostMoveSelection       ( MicroMeterPoint const & );
	void PostMoveShape           ( ShapeId const, MicroMeterPoint const & );
	void PostRemoveBeepers       ( );
	void PostRemoveShape         ( ShapeId const );
	void PostResetModel          ( );
	void PostResetTimer          ( );
	void PostSelectAll           ( tBoolOp const );
	void PostSelectAllBeepers    ( );
	void PostSelectShape         ( ShapeId const, tBoolOp const );
	void PostSelectShapesInRect  ( MicroMeterRect const & );
	void PostSelectSubtree       ( ShapeId const, tBoolOp const );
	void PostSendBack            ( int const );
	void PostSetParameter        ( tParameter const, float const );
	void PostSetPulseRate        ( ShapeId    const, fHertz const );
	void PostSetTriggerSound     ( ShapeId const, bool const, Hertz const, MilliSecs const );

	bool IsAsyncThread( ) const { return m_pNNetWorkThread->IsAsyncThread( ); }

	wchar_t const * GetActionCommandName    ( int const ) const;
	int     const   GetActionCommandFromName( wchar_t const * const ) const;

private:

	bool       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

	bool              m_bTrace;
	wostream        * m_pTraceStream    { nullptr };
	NNetWorkThread  * m_pNNetWorkThread { nullptr };

	void postMsg( NNetWorkThreadMessage::Id msg, WPARAM const = 0, LPARAM const = 0 );
}; 
