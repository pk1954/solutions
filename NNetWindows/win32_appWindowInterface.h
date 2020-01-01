// win32_appWindowInterface.h
//
// NNetSimu

#pragma once

#include "win32_baseAppWindow.h"

class AppWindowInterface : public BaseAppWindow
{

public:
	~AppWindowInterface( ) { };

	virtual void Start( ) = 0;
};
