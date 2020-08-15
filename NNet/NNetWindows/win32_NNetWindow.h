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
class DrawModel;
class Observable;
class ActionTimer;
class ObserverInterface;
class ControllerInterface;
class NNetModelWriterInterface;
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
		DrawModel                * const,
		Observable               * const
	);

	void Stop( );

	virtual ~NNetWindow( );

	MicroMeterRect const GetViewRect() const;

protected:

	DrawModel      const * GetDrawModel    () const { return m_pDrawModel; }
	PixelCoordsFp  const & GetCoord        () const { return m_context.GetCoordC(); }
	MicroMeterRect const   GetEnclosingRect() const;

	virtual bool OnCommand    ( WPARAM const, LPARAM const, PixelPoint const );
	virtual void OnSize       ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown( WPARAM const, LPARAM const ) {}
	virtual void OnPaint( );

	virtual void doPaint( ) = 0;

	PixelPoint m_ptLast { PP_NULL };	// Last cursor position during selection 

	D2D_DrawContext            m_context               { };
	NNetModelReaderInterface * m_pModelReaderInterface { nullptr };

private:

	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	DrawModel      * m_pDrawModel  { nullptr };
	NNetController * m_pController { nullptr };
};
