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

	bool const SetFontSize(int const);
	int  const GetFontSize() { return m_iFontSize; }

	virtual void       ClearDescription();
	virtual void       SetDescription(wstring const);
	virtual int  const GetLineCount() const;
    virtual bool const GetDescriptionLine(int const, wstring &) const;
	virtual bool const IsDirty() { return m_bDirty; };
	virtual void       ResetDirtyFlag() { m_bDirty = false; };

private:
	int   m_iFontSize { 20 };
	HFONT m_hFont     { nullptr };
	HWND  m_hwndEdit  { nullptr };
	bool  m_bDirty    { false };

	virtual void OnPaint  () { };
	virtual bool OnSize   (WPARAM const, LPARAM const);
	virtual bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL());

	bool const delChar();
	void fontSize();

	friend static LRESULT CALLBACK OwnerDrawEditBox(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
};
