// PerformanceWindow.cpp :
//
// NNetWindows

module;

#include <iomanip>
#include <sstream>
#include <Windows.h>

module NNetWin32:PerformanceWindow;

import SlowMotionRatio;
import HiResTimer;
import NNetModel;
import :Compute;

using std::wostringstream;
using std::setprecision;
using std::fixed;

void PerformanceWindow::Start
(
	HWND                    const hwndParent,
	Compute               * const pCompute,
	SlowMotionRatio const * const pSlowMotionRatio,
	HiResTimer            * const pDisplayTimer
)
{
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 0_PIXEL, 300_PIXEL, 350_PIXEL }, 
		L"PerformanceWindow", 
		100,  // alpha
		nullptr
	);
	m_pSlowMotionRatio = pSlowMotionRatio;
	m_pCompute         = pCompute;
	m_pDisplayTimer    = pDisplayTimer;
}

void PerformanceWindow::Stop()
{
	TextWindow::StopTextWindow();
	Show(false);
}

void PerformanceWindow::SetModelInterface(NNetModelReaderInterface const * const pNMRI)
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
		microseconds const usDisplayTime = PerfCounter::TicksToMicroseconds(m_pDisplayTimer->GetSingleActionTicks());
		textBuf.printString(L"Display:");
		textBuf.printString(L"");
		textBuf.printAsMillisecs(usDisplayTime);
		textBuf.nextLine();
	}

	if (m_pCompute)
	{
		fMicroSecs avail { m_pCompute->GetTimeAvailPerCycle() };
		//fMicroSecs spent { m_pCompute->GetTimeSpentPerCycle() };
		printMicroSecLine(textBuf, L"simu time res:", m_pCompute->GetSimuTimeResolution());
		//printFloatLine   (textBuf, L"targ slowmo:",   m_pSlowMotionRatio->GetNominalSlowMo(), L"");
		//if (avail > 0._MicroSecs)
		//{
		//	printMicroSecLine(textBuf, L"avail time:", avail);
		//	printMicroSecLine(textBuf, L"spent time:", spent);
		//	printFloatLine   (textBuf, L"workload:",   Cast2Float((spent / avail) * 100.0f), L"%");
		//}
//		printFloatLine   (textBuf, L"effect slomo:",  m_pCompute->GetEffectiveSlowmo(), L"");
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
