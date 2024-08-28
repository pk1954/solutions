// PerformanceWindow.ixx
//
// EvoWindows

export module PerformanceWindow;

import TextWindow;
//import ActionTimer;
import Delay;

export class PerformanceWindow: public TextWindow
{
public:
    PerformanceWindow();
    ~PerformanceWindow() {}; 

    void Start
	(
		HWND const, 
		Delay &, 
		//ActionTimer &, TODO
		//ActionTimer &,
		function<bool()> const 
	);

	void Stop();

    virtual void DoPaint(TextBuffer &);

private:
	void printLine(TextBuffer &, wchar_t const * const, microseconds const);
	void printLine(TextBuffer &, wchar_t const * const, Hertz        const);

	//ActionTimer * m_pAtComputation;    //TODO
	//ActionTimer * m_pAtDisplay;
	Delay       * m_pDelay;
};
