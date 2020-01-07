// win32_NNetWorkThreadInterface.h
//
// NNetWindows

#pragma once

#include <fstream>
#include "boolOp.h"
#include "Shape.h"
#include "NNetModel.h"
#include "win32_NNetWorkThread.h"

using std::wostream;

class ActionTimer;
class ObserverInterface;
class EventInterface;
class SlowMotionRatio;

class NNetWorkThreadInterface
{
public:
	NNetWorkThreadInterface( );
    ~NNetWorkThreadInterface( );

	void Initialize( std::wostream * );

	void Start
    ( 
		HWND                const,
		ActionTimer       * const,
		EventInterface    * const,
		ObserverInterface * const,
		SlowMotionRatio   * const,
		NNetModel         * const,
		BOOL                const
	);
	void Stop(); 

	void PostSetPulseRate     ( ShapeId    const, float const );
	void PostSetParameter     ( tParameter const, float const );
	void PostResetTimer       ( );
	void PostConnect          ( ShapeId const, ShapeId const );
	void PostRemoveShape      ( ShapeId const );
	void PostDisconnect       ( ShapeId const );
	void PostSlowMotionChanged( );
	void PostMoveShape        ( ShapeId const, MicroMeterPoint const & );
	void PostActionCommand    ( int const, ShapeId const, MicroMeterPoint const & );
	void PostResetModel       ( );
	void PostRunGenerations   ( BOOL const );
	void PostStopComputation  ( );
	void PostGenerationStep   ( );
	void PostRepeatGenerationStep();       // Do not call! Used by WorkThread only;

	BOOL   IsRunning    ( ) const { return m_pNNetWorkThread->IsRunning    ( ); }
	BOOL   IsAsyncThread( ) const { return m_pNNetWorkThread->IsAsyncThread( ); }
	double GetDutyCycle ( ) const { return m_pNNetWorkThread->GetDutyCycle ( ); }

	wchar_t const * GetActionCommandName    ( int const ) const;
	int     const   GetActionCommandFromName( wchar_t const * const ) const;

	void AddRunObserver        ( ObserverInterface * p ) { m_pNNetWorkThread->AddRunObserver        ( p ); }
	void AddPerformanceObserver( ObserverInterface * p ) { m_pNNetWorkThread->AddPerformanceObserver( p ); }

private:

	BOOL       IsTraceOn  ( ) const { return   m_bTrace; }
	wostream & TraceStream( )       { return * m_pTraceStream; }

	wostream        * m_pTraceStream;
	BOOL              m_bTrace;
	NNetWorkThread  * m_pNNetWorkThread;
	NNetModel const * m_pModel;
}; 
