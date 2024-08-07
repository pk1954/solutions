// ScanDataPoint.ixx
//
// NNetModel

export module NNetModel:ScanDataPoint;

import :Pipe;

export class ScanDataPoint
{
public:

    ScanDataPoint(Pipe const& pipe, Pipe::SegNr const segNr)
      : m_pPipe(&pipe),
        m_segNr(segNr)
    {
        Check();
    }

    void Check() const
    {
        //assert(m_segNr.GetValue() < m_pPipe->GetNrOfSegments());
    }

    mV GetSignalValue() const
    {
        Check();
        return m_pPipe->GetVoltage(m_segNr);
    }

    Pipe const& GetPipe() const
    {
        Check();
        return *m_pPipe;
    }

private:
    Pipe        const* m_pPipe { nullptr };
    Pipe::SegNr const  m_segNr;
};

