// Win32_Status.ixx
//
// NNetWindows

module;

#include <string>
#include <vector>
#include <Windows.h>
#include "NNetModelWriterInterface.h"
#include "Script.h"

export module StatusBar;

import PixelTypes;
import RootWindow;

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
	HWND WINAPI AddButton       (LPCTSTR const, HMENU const, DWORD const);
	HWND WINAPI AddTrackBar     (HMENU const);

	void AddCustomControl(int const);

	int  NewPart();
	void LastPart();

	void DisplayInPart(int const, wstring const &) const;
	void ClearPart(int const) const;

	void ReadProgressReport (int const, Script *) const;
	void WriteProgressReport(int const, wstring const &) const;

private:

	inline static PIXEL const STATUS_BAR_HEIGHT { 22_PIXEL };

	HWND WINAPI addControl(LPCTSTR const, LPCTSTR const, int const, DWORD const, HMENU const);

	vector<PIXEL> m_statWidths;

	PIXEL m_pixClientHeight { 0_PIXEL };
	PIXEL m_pixBorderX      { 0_PIXEL };
	PIXEL m_pixBorderY      { 0_PIXEL };
	PIXEL m_pixPosX         { 0_PIXEL };

	virtual LRESULT UserProc(UINT const, WPARAM const, LPARAM const);

	friend static LRESULT CALLBACK OwnerDrawStatusBar(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
};
