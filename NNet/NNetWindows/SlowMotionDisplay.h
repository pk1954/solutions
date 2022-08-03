// SlowMotionDisplay.h
//
// NNetSimu

#include <string>

import ObserverInterface;

class StatusBar;
class SlowMotionRatio;

using std::wstring;

class SlowMotionDisplay : public ObserverInterface
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
