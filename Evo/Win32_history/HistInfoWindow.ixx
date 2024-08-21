// HistInfoWindow.ixx
//
// Win32_history

export module WinHistory:HistInfoWindow;

import std;
import TextWindow;

using std::function;

class HistorySystem;

class HistInfoWindow: public TextWindow
{
public:
    HistInfoWindow();

	void Start(	HWND const, function<bool()> const);
	void Stop();

	void SetHistorySystem(HistorySystem *);

    virtual void DoPaint(TextBuffer &);

private:
	HistorySystem * m_pHistSys;
};
