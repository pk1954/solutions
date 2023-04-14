// CrsrWindow.ixx
//
// NNetWindows

module;

#include <compare>
#include <iostream>
#include <iomanip>
#include <string>
#include <Windows.h>

export module NNetWin32:CrsrWindow;

import Types;
import TextBuffer;
import TextWindow;
import NNetModel;
import :MainWindow;

using std::wostringstream;
using std::setprecision;
using std::wstring;
using std::fixed;

export class CrsrWindow: public TextWindow
{
public:
	CrsrWindow();
	~CrsrWindow() final;

	void Start(HWND const, MainWindow const * const);
	void Stop();
	void SetModelInterface(NNetModelReaderInterface * const);
	void PaintText(TextBuffer &) final;

private:
	MainWindow               const * m_pMainWindow { nullptr };
	NNetModelReaderInterface const * m_pNMRI       { nullptr };

	wstring GetCaption() const final { return L"CrsrWindow"; }

	void printPositionInfo(TextBuffer &, MicroMeterPnt const &)                 const;
	void printNobInfo     (TextBuffer &, MicroMeterPnt const &, NobId    const) const;
	void printSignalInfo  (TextBuffer &, SignalId const) const;

	void printMicroMeter(TextBuffer &, MicroMeter const) const;
	void printMilliSecs (TextBuffer &, MilliSecs  const) const;
	void printDegrees   (TextBuffer &, Degrees    const) const;
	void printVoltage   (TextBuffer &, mV         const) const;

	void printFrequency(TextBuffer & textBuf, Hertz const freq) const
	{
		wostringstream wBuffer;
		wBuffer << fixed << setprecision(1) << freq.GetValue() << L"Hz ";
		textBuf.printString(wBuffer.str());
		textBuf.nextLine();
	}
};
