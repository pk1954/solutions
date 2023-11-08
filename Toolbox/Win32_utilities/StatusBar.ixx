// Status.ixx
//
// Win32_utilities

module;

#include <string>
#include <vector>
#include <Windows.h>

export module StatusBar;

import Types;
import RootWindow;
import Script;

using std::wstring;
using std::vector;

export class StatusBar : public RootWindow
{
public:

	void Start(HWND const);
	void Stop();

	PIXEL GetHeight() const;
	void  Resize() const;

	HWND WINAPI AddStaticControl(int const);
	HWND WINAPI AddStaticControl(LPCTSTR const);
	HWND WINAPI AddButton       (LPCTSTR const, int const, DWORD const);
	HWND WINAPI AddTrackBar     (int const);

	void AddCustomControl(int const);

	int  NewPart();
	void LastPart();

	void DisplayInPart(int const, wstring const &) const;
	void ClearPart(int const) const;

	void ReadProgressReport (int const, Script *) const;
	void WriteProgressReport(int const, wstring const &) const;

private:

	inline static PIXEL const STATUS_BAR_HEIGHT { 22_PIXEL };

	HWND WINAPI addControl(LPCTSTR const, LPCTSTR const, int const, DWORD const, int const);

	vector<PIXEL> m_statWidths;

	PIXEL m_pixClientHeight { 0_PIXEL };
	PIXEL m_pixBorderX      { 0_PIXEL };
	PIXEL m_pixBorderY      { 0_PIXEL };
	PIXEL m_pixPosX         { 0_PIXEL };

	virtual LRESULT UserProc(UINT const, WPARAM const, LPARAM const);

	friend static LRESULT CALLBACK OwnerDrawStatusBar(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
};
