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
#include "InputLine.h"
#include "Pipe.h"
#include "Signal.h"
#include "Sensor.h"
#include "win32_util.h"
#include "win32_MainWindow.h"
#include "win32_crsrWindow.h"

import Win32_TextBuffer;

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
	HWND       const         hwndParent,
	MainWindow const * const pNNetWindow
) 
{
	m_pMainWindow = pNNetWindow;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 525_PIXEL }, 
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

void CrsrWindow::SetModelInterface(NNetModelReaderInterface * const pNMRI)
{
	m_pNMRI = pNMRI;
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
	MicroMeterPnt umPntCrsr { m_pMainWindow->GetCursorPos() };
	printPositionInfo(textBuf, umPntCrsr);

	if (m_pNMRI)
	{
		SignalId const sigId { m_pNMRI->GetHighlightedSignalId() };
		NobId    const nobId { m_pMainWindow->GetHighlightedNobId() };
		if (IsDefined(nobId))
		{
			printNobInfo(textBuf, umPntCrsr, nobId);
		}
		if (sigId.IsValid())
		{
			printSignalInfo(textBuf, sigId);
		}
	}
}

void CrsrWindow::printPositionInfo
(
	TextBuffer          & textBuf, 
	MicroMeterPnt const & umPoint
) const
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

void CrsrWindow::printNobInfo
(
	TextBuffer          & textBuf, 
	MicroMeterPnt const & umPoint, 
	NobId         const id
) const 
{
	NobType const type { m_pNMRI->GetNobType(id) };
	textBuf.nextLine();
	textBuf.AlignRight(); 
	textBuf.printString(L"Nob #");
	textBuf.AlignLeft();  
	textBuf.printNumber(id.GetValue());
	if (m_pNMRI->IsSelected(id))
		textBuf.printString(L" selected");
	textBuf.nextLine();
	textBuf.AlignRight(); 
	textBuf.printString(L"type:"); 
	textBuf.AlignLeft();  
	textBuf.printString(NobType::GetName(type.GetValue())); 
	textBuf.nextLine();

	if (type.IsPipeType())
	{
		textBuf.AlignRight(); 
		textBuf.printString(L"# segments:");
		textBuf.AlignLeft();  
		textBuf.printNumber(m_pNMRI->GetNrOfSegments(id));
	}
	if (type.IsAnyNeuronType())
	{
		SoundDescr sound { m_pNMRI->GetTriggerSound(id) };
		if (sound.m_bOn)
		{
			textBuf.AlignRight(); 
			textBuf.nextLine(L"trigger sound:");
			textBuf.AlignLeft();  
			printFrequency(textBuf, sound.m_frequency);
			printMilliSecs(textBuf, sound.m_duration);
			textBuf.printString(L"msec");
			textBuf.nextLine();
		}
	}
	if (type.IsIoConnectorType())
	{
		textBuf.AlignRight(); 
		textBuf.printString(L"direction: ");
		textBuf.AlignLeft();  
		printDegrees(textBuf, m_pNMRI->GetDirection(id));
		textBuf.nextLine();
	}
 	mV potential 
	{ 
		type.IsPipeType()
		? m_pNMRI->GetVoltageAt(id, m_pMainWindow->GetCursorPos())
		: m_pNMRI->GetVoltage(id)
	};
	textBuf.AlignRight(); 
	textBuf.nextLine(L"potential ");
	textBuf.AlignLeft();  
	printVoltage(textBuf, potential);
	textBuf.nextLine();
}

void CrsrWindow::printSignalInfo
(
	TextBuffer   & textBuf, 
 	SignalId const id
) const
{
	if (Signal const * pSignal { m_pNMRI->GetMonitorDataC().GetConstSignalPtr(id) })
	{
		textBuf.nextLine();
		textBuf.AlignRight(); 
		textBuf.printString(L"SIgnal in track nr ");
		textBuf.printNumber(id.GetTrackNr().GetValue());
		textBuf.nextLine();
		if (pSignal->GetSigSrcType() == Signal::SIGSRC_CIRCLE)
		{
			Sensor const * psigSrc { static_cast<Sensor const *>(pSignal->GetSignalSource()) };
			textBuf.AlignRight(); 
			textBuf.printString(L"EEG-Sensor at ");
			printMicroMeter(textBuf, psigSrc->GetCenter().GetX()); 
			printMicroMeter(textBuf, psigSrc->GetCenter().GetY()); 
			textBuf.nextLine();
			textBuf.printString(L"Radius: ");
			printMicroMeter(textBuf, psigSrc->GetRadius()); 
			textBuf.nextLine();
			textBuf.AlignRight(); 
			textBuf.printString(L"Data points: ");
			textBuf.printString(to_wstring(psigSrc->GetNrOfElements())); 
			textBuf.nextLine();
			textBuf.AlignRight(); 
			textBuf.printString(L"Factor: ");
			textBuf.printString(to_wstring(psigSrc->GetDistFactor(m_pMainWindow->GetCursorPos()))); 
			textBuf.nextLine();
		}
	}
}
