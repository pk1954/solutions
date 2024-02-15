// CrsrWindow.cpp :
//
// NNetWindows

module;

#include <optional>
#include <iostream>
#include <sstream> 
#include <string> 
#include <iomanip>
#include <Windows.h>

module NNetWin32:CrsrWindow;

import Types;
import TextBuffer;
import Win32_Util;
import NNetModel;
import :MainWindow;

using std::optional;
using std::wostringstream;
using std::setprecision;
using std::to_wstring;
using std::fixed;

CrsrWindow::CrsrWindow() :
	TextWindow()
{}

CrsrWindow::~CrsrWindow()
{
	m_pMainWindow = nullptr;
}

void CrsrWindow::Start
(
	HWND               const hwndParent,
	MainWindow const * const pMainWindow,
	ScanMatrix       * const pScanMatrix
) 
{
	m_pMainWindow = pMainWindow;
	m_pScanMatrix = pScanMatrix;
	StartTextWindow
	(
		hwndParent, 
		PixelRect { 0_PIXEL, 300_PIXEL, 300_PIXEL, 525_PIXEL }, 
		L"CrsrWindow", 
		100, 
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

void CrsrWindow::printMicroMeter(TextBuffer & textBuf, MicroMeter const um)
{
	wostringstream wBuffer;
	MicroMeter     umAbs { abs(um.GetValue()) };
	wBuffer << fixed << setprecision(1);
	if (umAbs < 1000.0_MicroMeter)
		wBuffer << um << L"um ";
	else if (umAbs < 1000000.0_MicroMeter)
		wBuffer << um.GetValue() / 1000.0f << L"mm ";
	else
		wBuffer << um.GetValue() / 1000000.0f << L"m ";
	textBuf.printString(wBuffer.str());
}

void CrsrWindow::printDegrees(TextBuffer & textBuf, Degrees const degrees)
{
	wostringstream wBuffer;
	wBuffer << degrees << L"°";
	textBuf.printString(wBuffer.str());
	textBuf.nextLine();
}

void CrsrWindow::printMilliSecs(TextBuffer & textBuf, MilliSecs const msec)
{
	wostringstream wBuffer;
	wBuffer << msec << L" msec";
	textBuf.printString(wBuffer.str());
}

void CrsrWindow::printVoltage(TextBuffer & textBuf, mV const voltage)
{
	wostringstream wBuffer;
	wBuffer << voltage << L"mV";
	textBuf.printString(wBuffer.str());
}

void CrsrWindow::PaintText(TextBuffer & textBuf)
{
	MicroMeterPnt umPntCrsr { m_pMainWindow->GetCursorPos() };
	printPositionInfo(textBuf, umPntCrsr);

	if (m_pNMRI)
	{
		SignalId const sigId { m_pNMRI->GetHighlightedSignalId() };
		NobId    const nobId { m_pMainWindow->GetHighlightedNobId() };

		if (m_pMainWindow->IsInOptimizeMode())
		{
			printScanAreaInfo(textBuf, umPntCrsr);
		}
		else if (IsDefined(nobId))
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
)
{
	if (umPoint.IsNull())
	{
		textBuf.AlignLeft();
		textBuf.printString(L"Cursor not in model window");
	}
	else
	{
		textBuf.printString(L"Position: ");
		printMicroMeter(textBuf, umPoint.GetX());
		printMicroMeter(textBuf, umPoint.GetY());
	}
	textBuf.nextLine();
}

void CrsrWindow::printScanAreaInfo
(
	TextBuffer         & textBuf,
	MicroMeterPnt const& umPoint
)
{
	m_pScanMatrix->Prepare();
	Raster const& raster { m_pNMRI->GetScanRaster() };
	textBuf.AlignRight();
	textBuf.printString(L"ScanArea: ");
	textBuf.printNumber(raster.RasterWidth());
	textBuf.printNumber(raster.RasterHeight());
	textBuf.nextLine();
	textBuf.printString(L"Pnts per pixel: ");
	textBuf.nextLine();
	textBuf.printString(L"Maximum: ");
	textBuf.printNumber(m_pScanMatrix->MaxNrOfDataPoints());
	textBuf.nextLine();
	textBuf.printString(L"Average: ");
	textBuf.printFloat(m_pScanMatrix->AverageDataPointsPerPixel());
	textBuf.nextLine();
	textBuf.printString(L"Variance: ");
	textBuf.printFloat(m_pScanMatrix->DataPointVariance());
	textBuf.nextLine();
	textBuf.nextLine();
	if (optional<RasterPoint> const rPntOpt = raster.FindRasterPos(umPoint))
	{
		textBuf.printString(L"Pixel: ");
		textBuf.printNumber(rPntOpt.value().m_x);
		textBuf.printNumber(rPntOpt.value().m_y);
		textBuf.nextLine();
		if (m_pScanMatrix->IsValid(rPntOpt.value()))
		{
			size_t const nrOfPnts { m_pScanMatrix->NrOfDataPntsInPixel(rPntOpt.value()) };
			textBuf.printString(L"# Pnts: ");
			textBuf.AlignLeft();  
			textBuf.printNumber(nrOfPnts);
			textBuf.nextLine();
		}
	}
	else
	{
		textBuf.AlignLeft();
		textBuf.printString(L"Cursor not in scan area");
		textBuf.nextLine();
	}
}

void CrsrWindow::printNobInfo
(
	TextBuffer          & textBuf, 
	MicroMeterPnt const & umPoint, 
	NobId         const   id
) 
{
	Nob     const& nob  { *m_pNMRI->GetConstNob(id) };
	NobType const  type { nob.GetNobType() };
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

	if (type.IsInputLineType())
	{
		InputLine       const* pInputLine { Cast2InputLine(&nob) };
		SignalGenerator const* pSigGen    { pInputLine->GetSigGenC() };
		textBuf.AlignRight();
		textBuf.printString(L"SigGen:");
		textBuf.AlignLeft();
		textBuf.printString(pSigGen->GetName());
	}
	if (type.IsPipeType())
	{
		textBuf.AlignRight();
		textBuf.printString(L"# segments:");
		textBuf.AlignLeft();
		textBuf.printNumber(m_pNMRI->GetNrOfSegments(id));
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
)
{
	if (NNetSignal const * pSignal { static_cast<NNetSignal const *>(m_pNMRI->GetMonitorDataC().GetConstSignalPtr(id)) })
	{
		textBuf.nextLine();
		textBuf.AlignRight(); 
		textBuf.printString(L"Signal in track nr ");
		textBuf.printNumber(id.GetTrackNr().GetValue());
		textBuf.nextLine();
		if (pSignal->GetSignalSource()->SignalSourceType() == NNetSignalSource::Type::macroSensor)
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
