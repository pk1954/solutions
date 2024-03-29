// NNetEvent.ixx
//
// NNetModel

module;

export module NNetModel:NNetEvent;

import Types;
import SimulationTime;
import Signals;

export enum class EventType
{
	run, stimulus, startScan, stopScan
};

export class NNetEvent
{
public:
    NNetEvent()
        : m_timeStamp(SimulationTime::Get())
    {}

    fMicroSecs GetTimeStamp() { return m_timeStamp; }

    virtual EventType Type() const = 0;

private:
    fMicroSecs m_timeStamp;
};

export class RunEvent : public NNetEvent
{
public:
    EventType Type() const final { return EventType::run; }
};

export class StimulusEvent : public NNetEvent
{
public:
    StimulusEvent(SigGenId const id)
        : m_sigGenId(id)
    {}

    SigGenId GetId() { return m_sigGenId; }

    EventType Type() const final { return EventType::stimulus; }

private:
    SigGenId m_sigGenId;
};

export class StartScanEvent : public NNetEvent
{
public:
    EventType Type() const final { return EventType::startScan; }
};

export class StopScanEvent : public NNetEvent
{
public:
    EventType Type() const final { return EventType::stopScan; }
};
