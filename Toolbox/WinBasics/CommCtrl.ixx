// CommCtrl.ixx
//
// Toolbox\WinBasics

module;

#include <windows.h>
#include <CommCtrl.h>

export module WinBasics:CommCtrl;

export 
{
    using ::DefSubclassProc;
    using ::SetWindowSubclass;
}

export void InitCommCtrl()
{
	INITCOMMONCONTROLSEX icex // load common control's DLL 
	{
		sizeof(INITCOMMONCONTROLSEX),
		ICC_STANDARD_CLASSES | 
		ICC_BAR_CLASSES | 
		ICC_TAB_CLASSES | 
		ICC_TREEVIEW_CLASSES  // for tooltips
	};
	bool bRes = InitCommonControlsEx(&icex);
	DWORD errCode = GetLastError();
}