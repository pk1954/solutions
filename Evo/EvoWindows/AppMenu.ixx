// AppMenu.ixx
//
// EvoWindows

export module AppMenu;

import WinBasics;
//import WorkThread;
import WinManager;

export class AppMenu
{
public:
	virtual ~AppMenu() {}

	virtual void Initialize
	(
		HWND const, 
		//WorkThread const * const, 
		WinManager const * const
	) = 0;
	virtual void AdjustVisibility() = 0;
	virtual void Start() = 0;
	virtual void Stop() = 0;
};
