// PerformanceWindow.ixx
//
// EvoWindows

export module PerformanceWindow;

import TextWindow;

export class PerformanceWindow: public TextWindow
{
public:
    PerformanceWindow();
    ~PerformanceWindow() {}; 

    void Start
	(
		HWND const, 
		function<bool()> const 
	);

	void Stop();

    virtual void DoPaint(TextBuffer &);

private:
	void printLine(TextBuffer &, wchar_t const * const, microseconds const);
	void printLine(TextBuffer &, wchar_t const * const, Hertz        const);
};
