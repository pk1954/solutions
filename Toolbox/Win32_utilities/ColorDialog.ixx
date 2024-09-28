// ColorDialog.ixx
//
// Toolbox\win32_utilities

export module ColorDialog;

import WinBasics;
export import Color;

export bool ColorDialog
(
	HWND const hwndOwner,
	COLORREF  &colorRef
)
{
	static COLORREF acrCustClr[16]; // array of custom colors 
	CHOOSECOLOR cc;                 // common dialog box structure 
	cc.lStructSize    = sizeof(cc);
	cc.hwndOwner      = hwndOwner;
	cc.hInstance      = 0;
	cc.rgbResult      = colorRef;
	cc.lpCustColors   = (LPDWORD)acrCustClr;
	cc.Flags          = CC_FULLOPEN | CC_RGBINIT; // | CC_ENABLEHOOK;
	cc.lpfnHook       = 0;
	cc.lpTemplateName = 0;

	bool bRes { ChooseColorW(&cc) == TRUE };
	if (bRes)
		colorRef = cc.rgbResult;
	return bRes;
}

