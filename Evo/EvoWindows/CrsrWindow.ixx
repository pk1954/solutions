// CrsrWindow.ixx
//
// EvoWindows

export module CrsrWindow;

import TextWindow;
import EvoReadBuffer;

class FocusPoint;

export class CrsrWindow: public TextWindow
{
public:
    CrsrWindow();
    virtual ~CrsrWindow();

    void Start
	(
		HWND            const, 
		EvoReadBuffer * const,
		FocusPoint    * const 
	);

	void Stop();

    virtual void DoPaint(TextBuffer &);

private:
    FocusPoint    * m_pFocusPoint;
	EvoReadBuffer * m_pReadBuffer;
};
