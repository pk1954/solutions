// AppWindowInterface.ixx
//
// EvoWindows

export module AppWindowInterface;

import BaseAppWindow;

export class AppWindowInterface : public BaseAppWindow
{

public:
	~AppWindowInterface() { };

	virtual void Start() = 0;
};
