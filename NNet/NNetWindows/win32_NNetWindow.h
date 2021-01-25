// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "ShapeId.h"
#include "D2D_DrawContext.h"
#include "tHighlightType.h"
#include "SmoothMoveFp.h"
#include "win32_NNetController.h"
#include "win32_modelWindow.h"

using std::wstring;
using std::function;

class Scale;
class Observable;
class ActionTimer;
class MonitorData;
class BeaconAnimation;
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
		HWND                       const, 
		DWORD                      const,
		bool                       const,
		NNetController           * const,
		NNetModelReaderInterface * const,
		BeaconAnimation          * const
	);

	void Stop( );

	virtual ~NNetWindow( );

	MicroMeterRect const GetViewRect() const;

	DrawContext          & GetDrawContext()       { return m_context; }
	PixelCoordsFp  const & GetCoordC     () const { return m_context.GetCoordC(); }
	PixelCoordsFp        & GetCoord      ()       { return m_context.GetCoord (); }
	MicroMeter     const   PixelSize     () const { return m_context.GetPixelSize(); }

	ShapeId const FindShapeAt         ( PixelPoint const &, ShapeCrit const & ) const;
	void          DrawInteriorInRect  ( PixelRect  const &, ShapeCrit const & ) const;
	void          DrawExteriorInRect  ( PixelRect  const &                    ) const;
	void          DrawNeuronTextInRect( PixelRect  const &                    ) const;
	void          DrawArrowsInRect    ( PixelRect  const &, MicroMeter const  ) const;
	void          DrawSensors         ( ) const;

protected:

	virtual void OnPaint( );
	virtual bool OnSize       ( WPARAM const, LPARAM const );
	virtual bool OnCommand    ( WPARAM const, LPARAM const, PixelPoint const );
	virtual void OnLButtonDown( WPARAM const, LPARAM const ) {}

	virtual void doPaint( ) = 0;

	void AnimateBeacon( fPixel const );

	PixelPoint m_ptLast { PP_NULL };	// Last cursor position during selection 

	NNetModelReaderInterface * m_pNMRI { nullptr };

private:

	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	D2D_DrawContext   m_context          { };
	NNetController  * m_pController      { nullptr };
	BeaconAnimation * m_pBeaconAnimation { nullptr };
};
