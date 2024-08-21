// SpeedControl.ixx
//
// win32_appFramework

export module SpeedControl;

import WorkThreadInterface;
import StatusBar;

export module SpeedControl;

import LogarithmicTrackbar;
import HistorySystem;

export class SpeedControl
{
public:

	static void Add(StatusBar * const, HistorySystem * const, long const, long const, long const);
	static void Adjust(bool const,	WorkThreadInterface * const);

private:
	static StatusBar     * m_pStatusBar;
	static HistorySystem * m_pHistorySystem;
};
