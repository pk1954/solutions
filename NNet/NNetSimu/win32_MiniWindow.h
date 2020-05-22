// win32_MiniWindow.h : 
//
// NNetWindows

#pragma once

#include "ObserverInterface.h"
#include "win32_MainWindow.h"

class MiniWindow : public NNetWindow
{
public:

	virtual void OnMouseWheel( WPARAM const, LPARAM const ) { };  // mini window cannot be zoomed 
	virtual void OnMouseMove ( WPARAM const, LPARAM const );

	virtual long AddContextMenuEntries( HMENU const ) { return 0; }
	
	void ObservedNNetWindow( MainWindow * const );

private:

	virtual void doPaint( );

	void adjust( );

	MainWindow * m_pObservedNNetWindow { nullptr }; // Observed NNetWindow (or nullptr)
};

