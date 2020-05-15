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

	virtual long AddContextMenuEntries( HMENU const, PixelPoint const ) { return 0; }
		
	void Observe( NNetWindow * const );

private:

	virtual void doPaint( );

	NNetWindow * m_pNNetWindowObserved { nullptr }; // Observed NNetWindow (or nullptr)
};

