// win32_crsrWindow.cpp :
//
// NNetWindows

#include "stdafx.h"
#include <iostream>
#include <sstream> 
#include <string> 
#include <iomanip>
#include "NNetModelReaderInterface.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "Pipe.h"
#include "win32_util.h"
#include "win32_textBuffer.h"
#include "win32_MainWindow.h"
#include "win32_crsrWindow.h"

using std::setprecision;
using std::to_wstring;

CrsrWindow::CrsrWindow() :
	TextWindow()
{}

CrsrWindow::~CrsrWindow()
{
	m_pMainWindow = nullptr;
}

void CrsrWindow::Start
(
	HWND                     const         hwndParent,
	MainWindow               const * const pNNetWindow,
	NNetModelReaderInterface const * const pModelInterface
) 
{
	m_pMainWindow = pNNetWindow;
	m_pNMRI       = pModelInterface;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 500_PIXEL }, 
		L"CrsrWindow", 
		100, 
		true,
		nullptr
	);
//	::CreateWindowToolTip(GetWindowHandle(), L"Cursor window");
}

void CrsrWindow::Stop()
{
	TextWindow::StopTextWindow();
	Show(false);
}

void CrsrWindow::printMicroMeter(TextBuffer & textBuf, MicroMeter const um) const
{
	wostringstream wBuffer;
	MicroMeter     umAbs { abs(um.GetValue()) };
	wBuffer << fixed << setprecision(1);
	if (umAbs < 1000.0_MicroMeter)
		wBuffer << um.GetValue() << L"um ";
	else if (umAbs < 1000000.0_MicroMeter)
		wBuffer << um.GetValue() / 1000.0f << L"mm ";
	else
		wBuffer << um.GetValue() / 1000000.0f << L"m ";
	textBuf.printString(wBuffer.str());
}

void CrsrWindow::printDegrees(TextBuffer & textBuf, Degrees const degrees) const
{
	wostringstream wBuffer;
	wBuffer << degrees.GetValue() << L"°";
	textBuf.printString(wBuffer.str());
	textBuf.nextLine();
}

void CrsrWindow::printMilliSecs(TextBuffer & textBuf, MilliSecs const msec) const
{
	wostringstream wBuffer;
	wBuffer << msec.GetValue() << L" msec";
	textBuf.printString(wBuffer.str());
}

void CrsrWindow::printVoltage(TextBuffer & textBuf, mV const voltage) const
{
	wostringstream wBuffer;
	wBuffer << voltage.GetValue() << L"mV";
	textBuf.printString(wBuffer.str());
}

void CrsrWindow::DoPaint(TextBuffer & textBuf)
{
	printPositionInfo(textBuf, m_pMainWindow->GetCursorPos());

	SignalId const sigId { m_pNMRI->GetHighlightedSignalId() };
	if (sigId.IsNotNull())
		printSignalInfo(textBuf, sigId);
	else
	{
		NobId const nobId { m_pMainWindow->GetHighlightedNobId() };
		if (IsDefined(nobId))
			printNobInfo(textBuf, nobId);
	}
}

void CrsrWindow::printPositionInfo(TextBuffer& textBuf, MicroMeterPnt const umPoint) const
{
	if (umPoint.IsNull())
	{
		textBuf.AlignLeft();
		textBuf.printString(L"Cursor not in model window");
		return;
	}

	textBuf.printString(L"Position: ");
	printMicroMeter(textBuf, umPoint.GetX());
	printMicroMeter(textBuf, umPoint.GetY());
	textBuf.nextLine();
}

void CrsrWindow::printSignalInfo(TextBuffer& textBuf, SignalId const id) const
{
	Signal const & signal { * m_pNMRI->GetConstMonitorData().GetConstSignalPtr(id) };
	textBuf.AlignRight(); textBuf.printString(L"Signal at ");
	printMicroMeter(textBuf, signal.GetCenter().GetX()); 
    printMicroMeter(textBuf, signal.GetCenter().GetY()); 
	textBuf.nextLine();
	textBuf.AlignRight(); 
	textBuf.printString(L"Radius ");
	printMicroMeter(textBuf, signal.GetRadius()); 
	textBuf.nextLine();
	textBuf.AlignRight(); textBuf.printString(L"#Segments ");
	textBuf.printString(to_wstring(signal.GetNrOfElements())); 
}

void CrsrWindow::printNobInfo(TextBuffer & textBuf, NobId const id) const 
{
	NobType const type { m_pNMRI->GetNobType(id) };

	textBuf.AlignRight(); textBuf.printString(L"Nob #");
	textBuf.AlignLeft();  textBuf.printNumber(id.GetValue());
	if (m_pNMRI->IsSelected(id))
		textBuf.printString(L" selected");
	textBuf.nextLine();
	textBuf.AlignRight(); textBuf.printString(L"type:"); 
	textBuf.AlignLeft();  textBuf.printString(NobType::GetName(type.GetValue())); 
	textBuf.nextLine();

	mV potential { 0.0_mV };
	if (type.IsPipeType())
	{
		textBuf.AlignRight(); textBuf.printString(L"# segments:");
		textBuf.AlignLeft();  textBuf.printNumber(m_pNMRI->GetNrOfSegments(id));
		potential = m_pNMRI->GetVoltageAt(id, m_pMainWindow->GetCursorPos());
	}
	else 
	{
		potential = m_pNMRI->GetVoltage(id);
	}

	textBuf.AlignRight(); textBuf.nextLine(L"potential ");
	textBuf.AlignLeft();  printVoltage(textBuf, potential);
	textBuf.nextLine();

	if (type.IsAnyNeuronType())
	{
		SoundDescr sound { m_pNMRI->GetTriggerSound(id) };
		if (sound.m_bOn)
		{
			textBuf.AlignRight(); textBuf.nextLine(L"trigger sound:");
			textBuf.AlignLeft();  printFrequency(textBuf, sound.m_frequency);
			printMilliSecs(textBuf, sound.m_duration);
			textBuf.printString(L"msec");
			textBuf.nextLine();
		}
	}
	if (type.IsIoConnectorType())
	{
		if (type.IsInputConnectorType())
		{
			textBuf.AlignRight(); textBuf.printString(L"pulse freq: ");
			textBuf.AlignLeft();  printFrequency(textBuf, m_pNMRI->GetActFrequency(id));
			textBuf.nextLine();
		}
		textBuf.AlignRight(); textBuf.printString(L"direction: ");
		textBuf.AlignLeft();  printDegrees(textBuf, m_pNMRI->GetDirection(id));
		textBuf.nextLine();
	}
}
