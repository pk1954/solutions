// Signal.cpp 
//
// Signals

module Signals:Signal;

import std;
import std.compat;
import Observable;
import Util;
import Types;
import IoUtil;
import DrawContext;
import :SignalSource;
import :SignalParameters;

using std::wcout;
using std::endl;
using std::wostream;
using std::make_unique;

class NNetModelIO;

Signal::Signal() 
{
    if (m_pDynModelObservable)
        m_pDynModelObservable->RegisterObserver(*this);
}

Signal::~Signal()
{
    if (m_pDynModelObservable)
        m_pDynModelObservable->UnregisterObserver(*this);
}

//bool Signal::operator==(Signal const & rhs) const
//{
//    if (&m_dynModelObservable != &rhs.m_dynModelObservable) return false;
//    if (&m_sigSource          != &rhs.m_sigSource)          return false;
//    if (m_timeStart           != rhs.m_timeStart)           return false;
//    if (m_data.size()         != rhs.m_data.size())         return false;
//    if (!(m_data              == rhs.m_data))               return false;
//    return true;
//}
// TODO

void Signal::Reset()    
{ 
    m_timeStart = 0.0_MicroSecs;
    m_data.clear();
}

//bool Signal::Includes(MicroMeterPnt const p) const 
//{ 
//    return m_sigSource.Includes(p); 
//}

void Signal::WriteSignalData(wostream & out) const
{
    size_t const iLast { m_data.size() };
    out << CURLY_OPEN_BRACKET << m_data.size() << NR_SEPARATOR;
    for (size_t i = 0; i < iLast; ++i)
    {
        if (i % 12 == 0)
            out << endl;
        out << m_data[i].GetValue();
        out << SPACE;
    }
    out << endl << CURLY_CLOSE_BRACKET << endl;
}

SIG_INDEX Signal::time2index
(
    SignalParameters const & param,
    SIMU_TIME                usSimu
) const
{
    if (usSimu < m_timeStart)
        return INVALID_SIG_INDEX;
    fMicroSecs const usSignal    { usSimu - m_timeStart };
    float      const fNrOfPoints { usSignal / param.TimeResolution() };
    SIG_INDEX  const index       { static_cast<SIG_INDEX>(roundf(fNrOfPoints)) };
    return index;
}

SIMU_TIME Signal::index2time
(
    SignalParameters const & param,
    SIG_INDEX       const   index
) const
{
    if (index < 0)
        return 0.0_MicroSecs;
    float      const fNrOfPoints { static_cast<float>(index) };
    fMicroSecs const usSignal    { param.TimeResolution() * fNrOfPoints };
    SIMU_TIME  const usSimu      { usSignal + m_timeStart };
    return usSimu;
}

mV Signal::GetDataPoint
(
    SignalParameters const & param,
    SIMU_TIME        const   usSimu
) const
{
    SIG_INDEX index { time2index(param, usSimu) };
    return (index < 0)
           ? mV::NULL_VAL()
           : GetVectorValue<mV>(index, m_data);
}

void Signal::Reserve(size_t const size) 
{
    m_data.reserve(size);
}

void Signal::Add2Signal(mV const val) 
{
    m_data.push_back(val);
}

SIMU_TIME Signal::FindNextMaximum
(
    SignalParameters const & param,
    SIMU_TIME        const   usSimu
) const
{
    SIG_INDEX index { time2index(param, usSimu) };
    if ((0 > index)||(index >= m_data.size()))
        index = INVALID_SIG_INDEX;
    if ((index > 0) && (m_data[index - 1] > m_data[index])) // falling values, go left
    {   
        while ((--index > 0) && (m_data[index-1] >= m_data[index]));
    }
    else   // climbing values, go right
    {
        while ((index < (m_data.size()-1)) && (m_data[index] <= m_data[index + 1]))
            ++index;
    }
    return index2time(param, index);
}

void Signal::CheckSignal() const 
{
#ifdef _DEBUG
#endif
}

void Signal::Dump() const
{
    wcout << L"time start:" << m_timeStart << endl;
}
