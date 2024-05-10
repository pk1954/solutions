// NNetEvent.ixx
//
// NNetModel

module;

#include <string>

export module NNetModel:NNetEvent;

import Types;
import SimulationTime;
import Signals;

using std::wstring;

export enum class EventType
{
	stimulus, startScan, stopScan
};

export class NNetEvent
{
public:
    NNetEvent(fMicroSecs const usTimeStamp)
        : m_timestamp(usTimeStamp)
    {}

    NNetEvent()
        : m_timestamp(SimulationTime::Get())
    {}

    fMicroSecs GetTimestamp() const { return m_timestamp; }

    virtual EventType Type() const = 0;

    wstring GetEventTypeName() const { return GetName(Type()); }

    static wstring   GetName(EventType const);
    static EventType GetTypeFromName(wstring const &);

private:
    fMicroSecs m_timestamp;
};

export class StimulusEvent : public NNetEvent
{
public:
    StimulusEvent(fMicroSecs const usTimeStamp, SigGenId const id)
      : NNetEvent(usTimeStamp),
        m_sigGenId(id)
    {}

    StimulusEvent(SigGenId const id)
        : m_sigGenId(id)
    {}

    SigGenId GetId() const { return m_sigGenId; }

    EventType Type() const final { return EventType::stimulus; }

private:
    SigGenId m_sigGenId;
};

export class StartScanEvent : public NNetEvent
{
public:
    using NNetEvent::NNetEvent;

    StartScanEvent(fMicroSecs const usTimeStamp)
      : NNetEvent(usTimeStamp)
    {}

    EventType Type() const final { return EventType::startScan; }
};

export class StopScanEvent : public NNetEvent
{
public:

    using NNetEvent::NNetEvent;

    StopScanEvent(fMicroSecs const usTimeStamp)
      : NNetEvent(usTimeStamp)
    {}

    EventType Type() const final { return EventType::stopScan; }
};
