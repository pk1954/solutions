// ColorDialog.ixx
//
// Toolbox\win32_utilities

module;

#include <Windows.h> 

export module ColorDialog;

export import Color;

export bool ColorDialog
(
	HWND const hwndOwner,
	COLORREF  &colorRef
)
{
	static COLORREF acrCustClr[16]; // array of custom colors 
	CHOOSECOLOR cc;                 // common dialog box structure 
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize  = sizeof(cc);
	cc.hwndOwner    = hwndOwner;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult    = colorRef;
	cc.Flags        = CC_FULLOPEN | CC_RGBINIT; // | CC_ENABLEHOOK;
	cc.lpfnHook     = 0;

	bool bRes { ChooseColor(&cc) == TRUE };
	if (bRes)
		colorRef = cc.rgbResult;
	return bRes;
}

