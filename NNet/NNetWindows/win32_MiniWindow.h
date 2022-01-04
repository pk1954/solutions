// win32_MiniWindow.h : 
//
// NNetWindows

#pragma once

#include "observerInterface.h"
#include "win32_NNetWindow.h"

class MiniWindow : public NNetWindow
{
public:

	void   OnMouseWheel         (WPARAM const, LPARAM const) final { };  // mini window cannot be zoomed 
	bool   OnRButtonUp          (WPARAM const, LPARAM const) final { return false; }
	bool   OnRButtonDown        (WPARAM const, LPARAM const) final { return false; }
	void   OnLButtonUp          (WPARAM const, LPARAM const) final { };
	void   OnLeftButtonDblClick (WPARAM const, LPARAM const) final { };
	void   OnChar               (WPARAM const, LPARAM const) final { };

	void   OnMouseMove          (WPARAM const, LPARAM const) final;

	LPARAM AddContextMenuEntries(HMENU const) final { return 0; }

	void   Notify(bool const) final;

	void ObservedNNetWindow(MainWindow * const);

private:

	void doPaint() final;

	MainWindow * m_pObservedNNetWindow { nullptr }; // Observed NNetWindow (or nullptr)
};

