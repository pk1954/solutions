// SlowMotionDisplay.ixx
//
// NNetSimu

module;

#include <string>

export module SlowMotionDisplay;

import ObserverInterface;
import StatusBar;
import SlowMotionRatio;

using std::wstring;

export class SlowMotionDisplay : public ObserverInterface
{
public:
	void Initialize(StatusBar *, SlowMotionRatio *, int);

	void Notify(bool const) final;

private:

	wstring           m_wstring          { };
	int               m_iPartInStatusBar { -1 };
	StatusBar       * m_pStatusBar       { nullptr };
	SlowMotionRatio * m_pSlowMotionRatio { nullptr };
};
