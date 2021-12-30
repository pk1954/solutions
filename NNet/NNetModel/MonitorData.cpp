// MonitorData.cpp 
//
// NNetWindows

#include "stdafx.h"
#include "Track.h"
#include "SignalFactory.h"
#include "win32_fatalError.h"
#include "MonitorData.h"

using std::distance;
using std::make_unique;
using std::move;
using std::endl;

bool MonitorData::operator==(MonitorData const & rhs) const
{
	return m_tracks == rhs.m_tracks;
}

MonitorData::MonitorData(const MonitorData& rhs)  // copy constructor
{
	for (auto const & upTrack : rhs.m_tracks)
		m_tracks.push_back(make_unique<Track>(*upTrack.get()));
}

MonitorData& MonitorData::operator=(MonitorData&& rhs) noexcept // move assignment operator
{
	if (this != &rhs)
	{
		m_tracks.clear();
		for (auto const & upTrack : rhs.m_tracks)
			m_tracks.push_back(make_unique<Track>(*upTrack.get()));
	}
	return * this;
}

void MonitorData::Reset()
{
	m_tracks.clear();
	m_idSigHighlighted.Set2Null();
	m_pHighSigObservable->NotifyAll(false);
}

int MonitorData::GetNrOfTracks() const
{
	return Cast2Int(m_tracks.size());
}

bool MonitorData::NoTracks() const
{
	return m_tracks.empty();
}

void MonitorData::InsertTrack(TrackNr const trackNr)
{
	auto track = make_unique<Track>();
	m_tracks.insert(m_tracks.begin() + trackNr.GetValue(), move(track));
}

unique_ptr<Signal> MonitorData::removeSignal(SignalId const & id)
{ 
	return getTrack(id.GetTrackNr())->RemoveSignal(id.GetSignalNr()); // getTrack may throw MonitorDataException
};

SignalId MonitorData::SetHighlightedSignal(SignalId const id)
{
	SignalId const signalIdOld { m_idSigHighlighted };
	m_idSigHighlighted = id;
	if (m_pHighSigObservable)
		m_pHighSigObservable->NotifyAll(false);
	return signalIdOld;
}

SignalId MonitorData::SetHighlightedSignal(Signal const &sigNew)
{
	return SetHighlightedSignal(FindSignalId([&sigNew](Signal const &s){ return &s == &sigNew; }));
}

SignalId MonitorData::SetHighlightedSignal(MicroMeterPnt const& umPos)
{
	return SetHighlightedSignal(FindSignalId([&umPos](Signal const &s){ return s.Includes(umPos); }));
}

SignalId MonitorData::ResetHighlightedSignal()
{
	return SetHighlightedSignal(SignalId::NULL_VAL());
}

void MonitorData::AddSignal
(
	SignalId   const & id, 
	unique_ptr<Signal> upSignal 
)
{
	assert(upSignal);
	getTrack(id.GetTrackNr())->AddSignal(move(upSignal),id.GetSignalNr()); // getTrack may throw MonitorDataException
}

SignalNr MonitorData::AddSignal
(
	TrackNr      const trackNr, 
	unique_ptr<Signal> upSignal 
)
{
	if (IsValid(trackNr))
		return getTrack(trackNr)->AddSignal(move(upSignal)); // getTrack may throw MonitorDataException 
	else
		throw MonitorDataException(*this, trackNr, L"AddSignal(upSignal)");
}

unique_ptr<Signal> MonitorData::DeleteSignal(SignalId const & id)
{
	assert(IsValid(id));
	if (id == m_idSigHighlighted)
		ResetHighlightedSignal();
	return removeSignal(id);
}

SignalNr MonitorData::AddSignal
(
	TrackNr          const   trackNr, 
	MicroMeterCircle const & umCircle 
)
{
	if (IsValid(trackNr))
		return AddSignal(trackNr, SignalFactory::MakeSignal(umCircle));
	else 
		throw MonitorDataException(*this, trackNr, L"AddSignal(upCircle)");
}

SignalNr MonitorData::MoveSignal(SignalId const & id, TrackNr const trackNrDst)
{
	assert(IsValid(id) && IsValid(trackNrDst));
	SignalNr sigNr { AddSignal(trackNrDst, removeSignal(id)) };
	if (id == m_idSigHighlighted)
		SetHighlightedSignal(SignalId(trackNrDst, sigNr));
	return sigNr;
}

void MonitorData::DeleteTrack(TrackNr const trackNr)
{
	if (IsValid(trackNr))
		m_tracks.erase(m_tracks.begin() + trackNr.GetValue());
	else 
		throw MonitorDataException(*this, trackNr, L"DeleteTrack");
}

Signal * MonitorData::GetSignalPtr(SignalId const & id)
{
	if (id.IsNull())
		return nullptr;
	TrackNr const trackNr { id.GetTrackNr() };
	if (IsValid(trackNr))
		return getTrack(trackNr)->GetSignalPtr(id.GetSignalNr());
	throw MonitorDataException(*this, trackNr, L"GetSignalPtr");
}

Signal const * MonitorData::GetConstSignalPtr(SignalId const & id) const
{
	return id.IsNull()
		   ? nullptr
	       : getTrack(id.GetTrackNr())->GetConstSignalPtr(id.GetSignalNr());
}

Signal * MonitorData::GetHighlightedSignal()
{
	TrackNr const trackNr { m_idSigHighlighted.GetTrackNr() };
	if (IsValid(trackNr))
		return getTrack(trackNr)->GetSignalPtr(m_idSigHighlighted.GetSignalNr());
	throw MonitorDataException(*this, trackNr, L"GetHighlightedSignal");
}

Signal const * MonitorData::GetHighlightedSignal() const
{
	return GetConstSignalPtr(m_idSigHighlighted);
}

bool MonitorData::IsValid(TrackNr const trackNr) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

bool MonitorData::IsValid(SignalId const & id) const
{
	return IsValid(id.GetTrackNr()) && getTrack(id.GetTrackNr())->IsValid(id.GetSignalNr());
}

void MonitorData::CheckTracks() const
{
#ifdef _DEBUG
	for (unique_ptr<Track> const & upTrack: m_tracks) 
		upTrack->CheckSignals(); 
#endif
}

Track const * MonitorData::getTrack(TrackNr const trackNr) const
{
	if (IsValid(trackNr))
		return m_tracks[trackNr.GetValue()].get();
	throw MonitorDataException(*this, trackNr, L"getTrack");
}

Track * MonitorData::getTrack(TrackNr const trackNr) // calling const version 
{
	return const_cast<Track *>(static_cast<const MonitorData&>(*this).getTrack(trackNr));
}

bool MonitorData::IsEmptyTrack(TrackNr const trackNr) const 
{ 
	if (IsValid(trackNr))
		return getTrack(trackNr)->IsEmpty(); 
	throw MonitorDataException(*this, trackNr, L"IsEmptyTrack");
}

Signal const * MonitorData::FindSensor(MicroMeterPnt const & umPos) const
{
	return FindSignal([&umPos](Signal const & s) { return s.Includes(umPos); });
}

void MonitorData::HandleException(MonitorDataException const & e)
{
	wcout << Scanner::COMMENT_START << L"MonitorWindow: " << e.m_wstrMessage << endl;
	wcout << Scanner::COMMENT_START << L"TrackNr: "       << e.m_trackNr << endl;
	FatalError::Happened(10, L"MonitorData failure");
}

void MonitorData::Dump() const
{
	wcout << L"NrTracks: " << m_tracks.size() << endl;
	wcout << '(' << endl;
	int iNr = 0;
	for (auto & it: m_tracks)
	{
		wcout << L"Track " << iNr++ << endl;
		it->Dump();
	}
	wcout << ')' << endl;
}
