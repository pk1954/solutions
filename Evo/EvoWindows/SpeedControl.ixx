// SpeedControl.ixx
//
// win32_appFramework

export module SpeedControl;

import WorkThread;
import StatusBar;
import HistoryLib;

export class SpeedControl
{
public:

	static void Add(StatusBar * const, HistorySystem * const, long const, long const, long const);
	static void Adjust(bool const,	WorkThread * const);

private:
	static StatusBar     * m_pStatusBar;
	static HistorySystem * m_pHistorySystem;
};
