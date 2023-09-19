// ScanDataPoint.ixx
//
// NNetScan

module;

export module NNetScan:ScanDataPoint;

import NNetModel;

class ScanDataPoint
{
public:

    ScanDataPoint(Pipe const& pipe, Pipe::SegNr const segNr)
      : m_pPipe(&pipe),
        m_segNr(segNr)
    {}

    mV GetSignalValue() const
    {
        return m_pPipe->GetVoltage(m_segNr);
    }

private:
    Pipe        const* m_pPipe { nullptr };
    Pipe::SegNr const  m_segNr;
};

