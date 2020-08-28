// win32_MiniWindow.h : 
//
// NNetWindows

#pragma once

#include "ObserverInterface.h"
#include "win32_MainWindow.h"

class MiniWindow : public NNetWindow
{
public:

	virtual void OnMouseWheel        ( WPARAM const, LPARAM const ) { };  // mini window cannot be zoomed 
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const ) { return false; }
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const ) { return false; }
	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const ) { };
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const ) { };
	virtual void OnSetCursor         ( WPARAM const, LPARAM const ) { };
	virtual void OnChar              ( WPARAM const, LPARAM const ) { };

	virtual void OnMouseMove         ( WPARAM const, LPARAM const );

	virtual long AddContextMenuEntries( HMENU const ) { return 0; }

	virtual void Notify( bool const );

	void ObservedNNetWindow( MainWindow * const );

private:

	virtual void doPaint( );

	MainWindow * m_pObservedNNetWindow { nullptr }; // Observed NNetWindow (or nullptr)
};

