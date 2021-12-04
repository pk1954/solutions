// win32_descriptionWindow.h : 
//
// NNetWindows

#pragma once

#include <string>
#include "DescriptionUI.h"
#include "win32_baseWindow.h"

using std::wstring;

class DescriptionWindow : public BaseWindow, public DescriptionUI
{
public:
	void Start(HWND const);
	void Stop();

	bool SetFontSize(int const);
	int  GetFontSize() const { return m_iFontSize; }

	int  GetLineCount      ()                     const final;
	bool GetDescriptionLine(int const, wstring &) const final;
	void ClearDescription  ()                           final;
	void SetDescription    (wstring const)              final;

	bool IsDirty()        final { return m_bDirty; };
	void ResetDirtyFlag() final { m_bDirty = false; };

private:
	int   m_iFontSize { 20 };
	HFONT m_hFont     { nullptr };
	HWND  m_hwndEdit  { nullptr };
	bool  m_bDirty    { false };

	void OnPaint  () final { /* all painting done by edit control */ };
	bool OnSize   (WPARAM const, LPARAM const) final;
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;

	bool delChar();
	void fontSize();

	friend static LRESULT CALLBACK OwnerDrawEditBox(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
};
