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
	~CrsrWindow() final;

	void Start
	(
		HWND                             const, 
		MainWindow               const * const,
		NNetModelReaderInterface const * const
	);

	void Stop();

	void DoPaint(TextBuffer &) final;

private:
	MainWindow               const * m_pMainWindow { nullptr };
	NNetModelReaderInterface const * m_pNMRI       { nullptr };

	void printPositionInfo(TextBuffer &, MicroMeterPnt const &)                 const;
	void printNobInfo     (TextBuffer &, MicroMeterPnt const &, NobId    const) const;
	void printSignalInfo  (TextBuffer &, MicroMeterPnt const &, SignalId const) const;

	void printMicroMeter(TextBuffer &, MicroMeter const) const;
	void printMilliSecs (TextBuffer &, MilliSecs  const) const;
	void printDegrees   (TextBuffer &, Degrees    const) const;
	void printVoltage   (TextBuffer &, mV         const) const;

	template <typename T> void printFrequency(TextBuffer & textBuf, T const freq) const
	{
		wostringstream wBuffer;
		wBuffer << fixed << setprecision(1) << freq.GetValue() << L"Hz ";
		textBuf.printString(wBuffer.str());
		textBuf.nextLine();
	}
};
