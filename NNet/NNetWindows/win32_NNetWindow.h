// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "ShapeId.h"
#include "D2D_DrawContext.h"
#include "tHighlightType.h"
#include "SmoothMoveFp.h"
#include "win32_animation.h"
#include "win32_NNetController.h"
#include "win32_modelWindow.h"

using std::wstring;
using std::function;
using std::make_unique;

class Scale;
class Observable;
class ActionTimer;
class MonitorWindow;
class ObserverInterface;
class ControllerInterface;
class NNetModelReaderInterface;

class NNetWindow : public ModelWindow
{
public:
	static void InitClass( ActionTimer * const );

	NNetWindow( );

	void Start
	( 
		HWND                     const, 
		DWORD                    const,
		bool                     const,
		fPixel                   const,
		NNetModelReaderInterface const &,
		MonitorWindow            const &,
		NNetController                 &
	);

	void Stop( );

	virtual ~NNetWindow( );

	MicroMeterRect const GetViewRect() const;

	DrawContext          & GetDrawContext()       { return m_context; }
	PixelCoordsFp  const & GetCoordC     () const { return m_context.GetCoordC(); }
	PixelCoordsFp        & GetCoord      ()       { return m_context.GetCoord (); }
	MicroMeter     const   PixelSize     () const { return m_context.GetPixelSize(); }

	void DrawInteriorInRect  ( PixelRect  const &, ShapeCrit const & ) const;
	void DrawExteriorInRect  ( PixelRect  const &                    ) const;
	void DrawNeuronTextInRect( PixelRect  const &                    ) const;
	void DrawArrowsInRect    ( PixelRect  const &, MicroMeter const  ) const;
	void DrawSensors         ( ) const;

protected:

	virtual void OnPaint      ( );
	virtual bool OnSize       ( WPARAM const, LPARAM const );
	virtual bool OnCommand    ( WPARAM const, LPARAM const, PixelPoint const );
	virtual void OnLButtonDown( WPARAM const, LPARAM const ) {}

	virtual void doPaint( ) = 0;

	void DrawBeacon( );

	PixelPoint m_ptLast { PP_NULL };	// Last cursor position during selection 

	NNetModelReaderInterface const * m_pNMRI    { nullptr };
	D2D_driver                       m_graphics { };

private:
	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	unique_ptr<Animation<float>> m_upBeaconAnimation;
	float                        m_fRelBeaconSize { 0.0f }; 

	MonitorWindow const * m_pMonitorWindow { nullptr };
	NNetController      * m_pController    { nullptr };
	D2D_DrawContext       m_context        { };
	fPixel                m_fPixRadiusLimit;
};
