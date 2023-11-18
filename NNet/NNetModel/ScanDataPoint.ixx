// ScanDataPoint.ixx
//
// NNetModel

module;

export module NNetModel:ScanDataPoint;

import :Pipe;

export class ScanDataPoint
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

    Pipe const& GetPipe() const
    {
        return *m_pPipe;
    }

private:
    Pipe        const* m_pPipe { nullptr };
    Pipe::SegNr const  m_segNr;
};

