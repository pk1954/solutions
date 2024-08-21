// AppWindowInterface.ixx
//
// Win32_appFramework

export module AppWindowInterface;

import BaseAppWindow;

class AppWindowInterface : public BaseAppWindow
{

public:
	~AppWindowInterface() { };

	virtual void Start() = 0;
};
