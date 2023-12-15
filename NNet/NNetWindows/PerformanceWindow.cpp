// PerformanceWindow.cpp :
//
// NNetWindows

module;

#include <iomanip>
#include <sstream>
#include <Windows.h>

module NNetWin32:PerformanceWindow;

import SlowMotionRatio;
import ActionTimer;
import NNetModel;
import :ComputeThread;

using std::wostringstream;
using std::setprecision;
using std::fixed;

void PerformanceWindow::Start
(
	HWND                    const hwndParent,
	ComputeThread         * const pComputeThread,
	SlowMotionRatio const * const pSlowMotionRatio,
	ActionTimer           * const pDisplayTimer
)
{
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 280_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		true,
		nullptr
	);
	m_pSlowMotionRatio = pSlowMotionRatio;
	m_pComputeThread   = pComputeThread;
	m_pDisplayTimer    = pDisplayTimer;
}

void PerformanceWindow::Stop()
{
	TextWindow::StopTextWindow();
	Show(false);
}

void PerformanceWindow::SetModelInterface(NNetModelReaderInterface * const pNMRI)
{
	m_pNMRI = pNMRI;
}

void PerformanceWindow::printMicroSecLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwch1, 
	fMicroSecs      const usDuration
) const
{
	wostringstream wBuffer;
	wBuffer << fixed << setprecision(1) << usDuration << L" µs";
	textBuf.printString(pwch1);
	textBuf.printString(L"");
	textBuf.printString(wBuffer.str());
	textBuf.nextLine   ();
}

void PerformanceWindow::printIntLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwchBefore, 
	int             const iPrintValue
) const
{
	wostringstream wBuffer;
	wBuffer << std::fixed << std::setprecision(1) << iPrintValue;
	textBuf.printString(pwchBefore);
	textBuf.printString(L"");
	textBuf.printString(wBuffer.str());
	textBuf.nextLine   ();
}

void PerformanceWindow::printFloatLine
(
	TextBuffer          & textBuf,
	wchar_t const * const pwchBefore, 
	float           const fPrintValue,
	wchar_t const * const pwchAfter
) const
{
	wostringstream wBuffer;
	wBuffer << std::fixed << std::setprecision(1) << fPrintValue << pwchAfter;
	textBuf.printString(pwchBefore);
	textBuf.printString(L"");
	textBuf.printString(wBuffer.str());
	textBuf.nextLine   ();
}

void PerformanceWindow::PaintText(TextBuffer & textBuf)
{      
	if (m_pDisplayTimer)
	{
		microseconds const usDisplayTime = m_pDisplayTimer->GetSingleActionTime();
		textBuf.printString(L"Display:");
		textBuf.printString(L"");
		textBuf.printAsMillisecs(usDisplayTime);
		textBuf.nextLine();
	}

	if (m_pComputeThread)
	{
		fMicroSecs avail { m_pComputeThread->GetTimeAvailPerCycle() };
		fMicroSecs spent { m_pComputeThread->GetTimeSpentPerCycle() };
		printMicroSecLine(textBuf, L"simu time res:", m_pComputeThread->GetSimuTimeResolution());
		printFloatLine   (textBuf, L"targ slowmo:",   m_pSlowMotionRatio->GetRatio(), L"");
		if (avail > 0._MicroSecs)
		{
			printMicroSecLine(textBuf, L"avail time:",    avail);
			printMicroSecLine(textBuf, L"spent time:",    spent);
			printFloatLine   (textBuf, L"workload:",      Cast2Float((spent / avail) * 100.0f), L"%");
		}
		//printFloatLine   (textBuf, L"effect slomo:",  m_pComputeThread->GetEffectiveSlowmo(), L"");
		if (m_pNMRI)
		{
			NobType::Apply2All
			(
				[this, &textBuf](NobType const & type)
				{ printIntLine(textBuf, (NobType::GetName(type.GetValue()) + L":").c_str(), m_pNMRI->GetNrOf(type)); }
			);
			printIntLine(textBuf, L"Nobs:", m_pNMRI->GetNrOfNobs());
		}
	}
}
