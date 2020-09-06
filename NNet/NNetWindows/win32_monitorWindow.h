// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include "D2D_DrawContext.h"
#include "win32_baseWindow.h"

class Param;
class Signal;
class NNetModelReaderInterface;

class MonitorWindow : public BaseWindow
{
public:
	void Start( HWND const, NNetModelReaderInterface const &, Param const & );
	void Stop( );

	void SetSignal( Signal const & );

	virtual long AddContextMenuEntries( HMENU const ) { return 0; }

private:

	virtual void OnPaint( );
	virtual bool OnSize       ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp  ( WPARAM const, LPARAM const ) { return false; }
	virtual bool OnRButtonDown( WPARAM const, LPARAM const ) { return false; }
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const ) { };
	virtual void OnSetCursor  ( WPARAM const, LPARAM const ) { };
	virtual void OnChar       ( WPARAM const, LPARAM const ) { };
	virtual void OnMouseMove  ( WPARAM const, LPARAM const ) { };

	void doPaint( );

	fPIXEL const getYvalue( fMicroSecs const time )
	{
		float  const fDataPoint { m_pSignal->GetDataPoint( time ) };
		fPIXEL const fPixYvalue { fDataPoint / m_fYvaluesPerPixel };
		return fPixYvalue;
	}

	D2D_driver                       m_graphics           { };
	Param                    const * m_pParams            { nullptr };
	NNetModelReaderInterface const * m_pModel             { nullptr };
	Signal                   const * m_pSignal            { nullptr };
	fMicroSecs                       m_fMicroSecsPerPixel { 100.0_MicroSecs };
	float                            m_fYvaluesPerPixel   {   0.2f };
};
