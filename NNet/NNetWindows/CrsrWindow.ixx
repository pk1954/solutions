// CrsrWindow.ixx
//
// NNetWindows

export module NNetWin32:CrsrWindow;

import std;
import Types;
import TextBuffer;
import TextWindow;
import NNetModel;
import NNetSignals;
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

	void Start
	(
		HWND const, 
		MainWindow    const * const, 
		MonitorWindow const * const
	);
	void SetModelInterface(NNetModelReaderInterface const * const);

	void PaintText(TextBuffer &) final;

private:
	MainWindow               const * m_pMainWindow    { nullptr };
	NNetModelReaderInterface const * m_pNMRI          { nullptr };
	MonitorWindow            const * m_pMonitorWindow { nullptr };

	wstring GetCaption() const final { return L"CrsrWindow"; }

	void printPositionInfo(TextBuffer&, MicroMeterPnt const &);
	void printNobInfo     (TextBuffer&, MicroMeterPnt const &, NobId const);
	void printSignalInfo  (TextBuffer&, SignalId      const );
	void printMicroMeter  (TextBuffer&, MicroMeter    const );
	void printMilliSecs   (TextBuffer&, MilliSecs     const );
	void printDegrees     (TextBuffer&, Degrees       const );
	void printVoltage     (TextBuffer&, mV            const );
	void printScanAreaInfo(TextBuffer&, RasterPoint   const &);
	void printFrequency   (TextBuffer&, Hertz         const );
};
