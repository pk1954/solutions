// win32_crsrWindow.h 
//
// NNetWindows

#pragma once

#include <iostream>
#include "win32_textWindow.h"

using std::wostringstream;
using std::setprecision;
using std::fixed;

class MainWindow;
class NNetModelReaderInterface;

class CrsrWindow: public TextWindow
{
public:
	CrsrWindow();
	virtual ~CrsrWindow();

	void Start
	(
		HWND                             const, 
		MainWindow               const * const,
		NNetModelReaderInterface const * const
	);

	void Stop();

	virtual void DoPaint(TextBuffer &);

private:
	MainWindow               const * m_pMainWindow { nullptr };
	NNetModelReaderInterface const * m_pNMRI       { nullptr };

	void printMicroMeter(TextBuffer &, MicroMeter const);
	void printMilliSecs (TextBuffer &, MilliSecs  const);
	void printDegrees   (TextBuffer &, Degrees    const);
	void printVoltage   (TextBuffer &, mV         const);

	template <typename T> void printFrequency(TextBuffer & textBuf, T const freq)
	{
		wostringstream wBuffer;
		wBuffer << fixed << setprecision(1) << freq.GetValue() << L"Hz ";
		textBuf.printString(wBuffer.str());
		textBuf.nextLine();
	}
};
