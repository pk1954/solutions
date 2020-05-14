// win32_MiniWindow.h : 
//
// NNetWindows

#pragma once

#include "win32_NNetWindow.h"

class MiniWindow : public NNetWindow
{
public:

	virtual ~MiniWindow();

	virtual void OnMouseWheel ( WPARAM const, LPARAM const ) { };  // mini window cannot be zoomed 
	virtual void OnMouseMove  ( WPARAM const, LPARAM const );
	virtual BOOL OnCommand    ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown( WPARAM const, LPARAM const );

	virtual long AddContextMenuEntries( HMENU const, PixelPoint const );

	virtual void doPaint( bool const );

	void Observe( NNetWindow * const );

private:

	NNetWindow * m_pNNetWindowObserved { nullptr }; // Observed NNetWindow (or nullptr)

	inline static UINT_PTR m_TimerId { 0 }; 
};

