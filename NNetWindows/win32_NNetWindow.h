// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "win32_modelWindow.h"

class ActionTimer;
class NNetWorkThreadInterface;

class NNetWindow : public ModelWindow
{
public:
	static void InitClass
	( 
		NNetWorkThreadInterface * const,
		ActionTimer             * const
	);

	NNetWindow( );

	void Start
	( 
		HWND  const, 
		DWORD const,
		std::function<bool()> const
	);
	void Stop( );

	~NNetWindow( );

private:
	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	static NNetWorkThreadInterface * m_pNNetWorkThreadInterface;

	BOOL  m_bMoveAllowed;    // TRUE: move with mouse is possible
	HMENU m_hPopupMenu;

	virtual void AddContextMenuEntries( HMENU const, POINT const );

	virtual void OnMouseWheel ( WPARAM const, LPARAM const );
	virtual void OnMouseMove  ( WPARAM const, LPARAM const );
	virtual BOOL OnCommand    ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown( WPARAM const, LPARAM const );
	virtual void OnLButtonUp  ( WPARAM const, LPARAM const );
	virtual void OnSetCursor  ( WPARAM const, LPARAM const );
	virtual void OnPaint( );

	BOOL inObservedClientRect( LPARAM const );
};
