// MiniWindow.ixx
//
// NNetWindows

export module NNetWin32:MiniWindow;

import std;
import Types;
import ObserverInterface;
import WinBasics;
import :MainWindow;
import :NNetCommandHandler;
import :NNetWindow;

using std::wstring;

export class MiniWindow : public NNetWindow
{
public:
	void Start(HWND const, bool const, NNetCommandHandler &);

	bool OnMouseWheel     (WPARAM const, LPARAM const) final { return true;  }  // mini window cannot be zoomed 
	bool OnRButtonUp      (WPARAM const, LPARAM const) final { return false; }
	bool OnRButtonDown    (WPARAM const, LPARAM const) final { return false; }
	bool OnLButtonUp      (WPARAM const, LPARAM const) final { return false; }
	void OnLButtonDblClick(WPARAM const, LPARAM const) final { };
	void OnMouseMove      (WPARAM const, LPARAM const) final;
		 
	void Trigger(bool const) final;
		 
	void ObservedNNetWindow(MainWindow * const);

private:

	wstring GetCaption() const final { return L"Mini window"; }

	void PaintGraphics() final;

	MainWindow * m_pObservedNNetWindow { nullptr }; // Observed NNetWindow (or nullptr)
};

