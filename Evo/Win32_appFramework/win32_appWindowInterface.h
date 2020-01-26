// win32_appWindowInterface.h
//
// Win32_appFramework

#pragma once

#include "win32_baseAppWindow.h"

class AppWindowInterface : public BaseAppWindow
{

public:
	~AppWindowInterface( ) { };

	virtual void Start( ) = 0;
};
