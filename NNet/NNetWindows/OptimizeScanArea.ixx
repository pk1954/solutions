// OptimizeScanArea.ixx
//
// NNetWindows

module;

#include <cassert>
#include <memory>

export module NNetWin32:OptimizeScanArea;

import SaveCast;
import Random;
import Raster;
import Thread;
import NNetModel;
import NNetCommands;
import UndoCommand;
import ScanMatrix;

using std::unique_ptr;
using std::make_unique;

export class OptimizeScanArea : public ::Thread
{
public:

	void SetModelInterface(NNetModelWriterInterface* const pNMWI)
	{
		m_pNMWI = pNMWI;
	}

	void Start()
	{
		BeginThread(L"OptimizeScanArea");
		m_rand.Initialize();
	}

	void Stop()
	{
		if (m_bRunning)
			Terminate();
		m_bRunning = false;
	}

	void ThreadMsgDispatcher(MSG const&) final { }

	void ThreadStartupFunc() final
	{
		Raster             const& raster         { m_pNMWI->GetScanRaster() };
		RasterPoint        const  rasterSize     { raster.Size() };
		MicroMeterRect     const  umRectScanArea { m_pNMWI->GetScanAreaRect() };
		MicroMeterRectSize const  umRectSize     { umRectScanArea.GetSize() };
		MicroMeterRectSize const  umRectMove     { umRectSize * 0.3f };

		m_bRunning = true;
		m_upScanMatrix = make_unique<ScanMatrix>(rasterSize);
		m_upScanMatrix->Fill(*m_pNMWI);
		m_fBestVariance = m_upScanMatrix->DataPointVariance();

		while (m_bRunning)
		{
			ScanDataPoint const& scanDataPnt { randomDataPoint() };
			Pipe          const& pipe        { scanDataPnt.GetPipe() };
			MicroMeter    const  umDeltaX    { randomDelta(umRectMove.GetX()) };
			MicroMeter    const  umDeltaY    { randomDelta(umRectMove.GetY()) };
			NobId                nobId       { m_rand.NextBooleanValue() ? pipe.GetStartKnotId() : pipe.GetEndKnotId() };
			while (m_pNMWI->HasParentNob(nobId))
				nobId = m_pNMWI->GetParentNob(nobId)->GetId();
			MoveNobCommand::Push(nobId, MicroMeterPnt(umDeltaX, umDeltaY));
			m_upScanMatrix->Clear();
			m_upScanMatrix->Fill(*m_pNMWI);
			float fVariance = m_upScanMatrix->DataPointVariance();
			if (fVariance <= m_fBestVariance)
			{
				m_fBestVariance = fVariance;
			}
			else
			{
				UndoCommand::Push();
			}
		}
	}

private:

	ScanDataPoint const& randomDataPoint() const
	{
		RasterPoint const   rasterSize { m_pNMWI->GetScanRaster().Size() };
		ScanPixel   const * pScanPixel;
		unsigned int nrOfDataPoints;
		//do
		//{
			//unsigned int uiX { m_rand.NextRandomNumberScaledTo(rasterSize.m_x) };
			//unsigned int uiY { m_rand.NextRandomNumberScaledTo(rasterSize.m_y) };
			//RasterPoint pntRandom(uiX, uiY);
			//pScanPixel = &m_upScanMatrix->GetScanPixel(pntRandom);
			pScanPixel = m_upScanMatrix->GetMaxScanPixel();
			assert(pScanPixel);
			nrOfDataPoints = Cast2UnsignedInt(pScanPixel->GetNrOfDataPoints());
		//} while (nrOfDataPoints == 0);
		return pScanPixel->GetScanDataPoint(m_rand.NextRandomNumberScaledTo(nrOfDataPoints));
	}

	MicroMeter randomDelta(MicroMeter const umMax) 
	{ 
		return umMax * 0.5f - umMax * Cast2Float(m_rand.NextRandomNumberScaledTo(100)) / 100.0f;
	}

	NNetModelWriterInterface *m_pNMWI { nullptr };
	unique_ptr<ScanMatrix>    m_upScanMatrix;
	mutable Random            m_rand;
	bool                      m_bRunning;
	float                     m_fBestVariance;
};
