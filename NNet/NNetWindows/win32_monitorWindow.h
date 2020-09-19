// win32_monitorWindow.h : 
//
// NNetWindows

#pragma once

#include <vector>
#include "D2D_DrawContext.h"
#include "win32_baseWindow.h"

using std::vector;

class Param;
class Signal;
class BeaconAnimation;
class NNetModelReaderInterface;

class MonitorWindow : public BaseWindow
{
public:
	void Start
	( 
		HWND const, 
		NNetModelReaderInterface const &, 
		Param                    const &,
		BeaconAnimation                &
	);

	void Stop( );

	void AddSignal( Signal & );

	virtual long AddContextMenuEntries( HMENU const );

private:

	virtual void OnPaint( );
	virtual bool OnSize       ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp  ( WPARAM const, LPARAM const ) { return false; }
	virtual bool OnRButtonDown( WPARAM const, LPARAM const ) { return false; }
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const ) { };
	virtual void OnChar       ( WPARAM const, LPARAM const ) { };
	virtual void OnMouseMove  ( WPARAM const, LPARAM const );
	virtual bool OnMouseLeave ( WPARAM const, LPARAM const );
	virtual bool OnCommand    ( WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL() );

	void doPaint( );
	void removeSignal( );
	void selectSignal( Signal * );
	void paintSignal( Signal const &, fPIXEL const, fPIXEL const, fMicroSecs const, fMicroSecs const, fMicroSecs const );

	fPIXEL const getYvalue( Signal const &, fMicroSecs const );
	PIXEL  const getSlotHeight( PIXEL );

	Signal * findSignal( );

	struct Slot
	{
		vector <Signal *> signals;
	};

	bool                             m_bRuler            { true };
	D2D_driver                       m_graphics          { };
	Param                    const * m_pParams           { nullptr };
	NNetModelReaderInterface const * m_pModel            { nullptr };
	BeaconAnimation                * m_pBeaconAnimation  { nullptr };
	vector <Slot>                    m_Slots             { };
	fMicroSecs                       m_fMicroSecsPerPixel{ 100.0_MicroSecs };
	float                            m_fYvaluesPerPixel  {   0.2f };
	Signal *                         m_pSelectedSignal   { nullptr };
	int                              m_iSelectedSlot     { -1 };
	TRACKMOUSEEVENT                  m_trackStruct       { sizeof(TRACKMOUSEEVENT), TME_LEAVE, nullptr, 0L };
};
