// SlowMotionDisplay.ixx
//
// Win32_utilities

module;

#include <string>
#include <sstream>
#include <memory>

export module SlowMotionDisplay;

import ObserverInterface;
import SlowMotionRatio;
import BaseRefreshRate;
import StatusBar;

using std::wstring;
using std::unique_ptr;
using std::wostringstream;

export class SlowMotionDisplay : public ObserverInterface
{
public:
	~SlowMotionDisplay();

	void Initialize(StatusBar *, int, SlowMotionRatio const &);

	void Notify(bool const) final;

	class RefreshRate : public StatusBarRefreshRate
	{
	public:
		RefreshRate(StatusBar *, int, SlowMotionRatio const &);
		void Trigger(bool const) final;

	private:
		wostringstream         m_wstrBuffer;
		SlowMotionRatio const &m_slowMotionRatio;
	};

private:

	unique_ptr<RefreshRate> m_upRefreshRate;
};
