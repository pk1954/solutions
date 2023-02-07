// MiniWindow.ixx
//
// NNetWindows

module;

#include <string>
#include <Windows.h>

export module NNetWin32:MiniWindow;

import Types;
import ObserverInterface;
import :MainWindow;
import :NNetController;
import :NNetWindow;

using std::wstring;

export class MiniWindow : public NNetWindow
{
public:
	void Start(HWND const, bool const, fPixel const, NNetController &);

	void   OnMouseWheel     (WPARAM const, LPARAM const) final { };  // mini window cannot be zoomed 
	bool   OnRButtonUp      (WPARAM const, LPARAM const) final { return false; }
	bool   OnRButtonDown    (WPARAM const, LPARAM const) final { return false; }
	bool   OnLButtonUp      (WPARAM const, LPARAM const) final { return false; };
	void   OnLButtonDblClick(WPARAM const, LPARAM const) final { };
	void   OnMouseMove      (WPARAM const, LPARAM const) final;

	LPARAM AddContextMenuEntries(HMENU const) final { return 0; }

	void   Notify(bool const) final;

	void   ObservedNNetWindow(MainWindow * const);

private:

	wstring GetCaption() const final { return L"Mini window"; }

	void DoPaint() final;

	MainWindow * m_pObservedNNetWindow { nullptr }; // Observed NNetWindow (or nullptr)
};

