// HistInfoWindow.cpp :
//
// Win32_history

module WinHistLib:HistInfoWindow;

import std;
import Types;
import WinBasics;
import HistoryLib;
import ObserverInterface;

using std::function;

HistInfoWindow::HistInfoWindow() :
    TextWindow(),
    m_pHistSys(nullptr)
{ }

void HistInfoWindow::Start(HWND const hwndParent, function<bool()> const visibilityCriterion) 
{
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 415_PIXEL }, 
		L"HistInfoWindow", 
		100, 
		visibilityCriterion
	);
}

void HistInfoWindow::Stop() 
{
	m_pHistSys->UnregisterAllObservers();
	m_pHistSys = nullptr;
}

void HistInfoWindow::SetHistorySystem(HistorySystem * pHistSys) 
{
	m_pHistSys = pHistSys;
	m_pHistSys->RegisterObserver(*this);  // Notify me, if something happens in history system
}

void HistInfoWindow::DoPaint(TextBuffer & textBuf)
{
	if (m_pHistSys)
	{
		int    iNrOfUsedSlots { m_pHistSys->GetNrOfUsedHistCacheSlots().GetValue() };
		int    iNrOfSlots     { m_pHistSys->GetNrOfHistCacheSlots().GetValue() };
		size_t slotSize       { m_pHistSys->GetSlotSize() };
		size_t totalSize      { slotSize * iNrOfSlots };

		textBuf.printString(L"used slots  ");
		textBuf.setHorizontalPos(3_TEXT_POSITION);
		textBuf.printPercentage(iNrOfUsedSlots, iNrOfSlots);

		textBuf.nextLine(L"slot size   ");
		textBuf.setHorizontalPos(3_TEXT_POSITION);
		textBuf.printNumber(slotSize);

		textBuf.nextLine(L"cache size  ");
		textBuf.setHorizontalPos(3_TEXT_POSITION);
		textBuf.printNumber(totalSize);

		textBuf.nextLine(L"genCurrent  ");
		textBuf.setHorizontalPos(3_TEXT_POSITION);
		textBuf.printNumber(m_pHistSys->GetCurrentGeneration().GetLong());
	}
}
