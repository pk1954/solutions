// StatusBar.ixx 
//
// Win32_appFramework

export module StatusBar;

import std;
import Types;
import WinBasics;
import RootWindow;
import WorkThreadInterface;

using std::wstring;
using std::vector;

class StatusBar : public RootWindow
{
public:
	StatusBar();

	void  Start(HWND const);
	void  Stop();

	PIXEL GetHeight() const;
    void  Resize() const;

    HWND AddStaticControl(LPCTSTR);
    HWND AddButton       (LPCTSTR const, HMENU const, DWORD const );
    HWND AddTrackBar     (HMENU);

	void AddCustomControl(PIXEL const);

	int  NewPart();
	void LastPart();

	void DisplayInPart(int const, std::wstring const &);

private:

	HWND addControl(LPCTSTR, LPCTSTR, DWORD, HMENU);

	vector<PIXEL> m_statWidths;

	PIXEL m_pixClientHeight;
    PIXEL m_pixBorderX;
    PIXEL m_pixBorderY;
    PIXEL m_pixPosX;

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);

friend static LRESULT __stdcall OwnerDrawStatusBar(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
};
