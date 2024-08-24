// Statistics.ixx
//
// EvoWindows

export module Statistics;

import WinBasics;
import EvolCoreLib;
import EvoReadBuffer;
import TextWindow;
import EvoStatistics;

export class StatisticsWindow : public TextWindow
{
public:
    StatisticsWindow();
    virtual ~StatisticsWindow();

    void Start(HWND const, EvoReadBuffer * const);
	void Stop();

    virtual void DoPaint(TextBuffer &);

private:
	EvoReadBuffer * m_pReadBuffer;
	EvoStatistics * m_pStatistics;
};
