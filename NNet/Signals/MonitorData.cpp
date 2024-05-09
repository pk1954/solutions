// MonitorData.cpp 
//
// Signals

module;

#include <cassert>
#include <memory>
#include <iterator>
#include <iostream>

module Signals:MonitorData;

import SaveCast;
import :Signal;

using std::distance;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::endl;
using std::wcout;

//bool MonitorData::operator==(MonitorData const & rhs) const
//{
//	return m_tracks == rhs.m_tracks;
//}
// TODO

void MonitorData::ResetMonitorData()
{
	m_tracks.clear();
}

void MonitorData::ClearDynamicData() 
{ 
	Apply2AllSignals([](Signal & s) { s.Reset(); });
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
	return getTrack(id.GetTrackNr())->RemoveSignal(id.GetSignalNr());
};

SignalNr MonitorData::AddSignal
(
	unique_ptr<Signal> upSignal, 
	TrackNr      const trackNr
)
{
	assert(upSignal);
	++m_iNrOfSignals;
	return getTrack(trackNr)->AddSignal(move(upSignal));
}

void MonitorData::AddSignal
(
	unique_ptr<Signal> upSignal,
	SignalId   const & id
)
{
	assert(upSignal);
	++m_iNrOfSignals;
	getTrack(id.GetTrackNr())->AddSignal(move(upSignal), id.GetSignalNr());
}

unique_ptr<Signal> MonitorData::RemoveSignal(SignalId const & id)
{
	assert(IsValid(id));
	--m_iNrOfSignals;
	return removeSignal(id);
}

SignalNr MonitorData::MoveSignal(SignalId const & id, TrackNr const trackNrDst)
{
	assert(IsValid(id) && IsValid(trackNrDst));
	SignalNr sigNr { AddSignal(removeSignal(id), trackNrDst) };
	return sigNr;
}

void MonitorData::DeleteTrack(TrackNr const trackNr)
{
	if (IsValid(trackNr))
		m_tracks.erase(m_tracks.begin() + trackNr.GetValue());
}

Signal * MonitorData::GetSignalPtr(SignalId const & id)
{
	if (id.IsNull())
		return nullptr;
	TrackNr const trackNr { id.GetTrackNr() };
	return (IsValid(trackNr))
		   ? getTrack(trackNr)->GetSignalPtr(id.GetSignalNr())
		   : nullptr;
}

Signal const * MonitorData::GetConstSignalPtr(SignalId const & id) const
{
	return (IsValid(id))
		   ? getTrack(id.GetTrackNr())->GetConstSignalPtr(id.GetSignalNr())
	       : nullptr;
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
	return (IsValid(trackNr))
		   ? m_tracks[trackNr.GetValue()].get()
	       : nullptr;
}

Track * MonitorData::getTrack(TrackNr const trackNr) // calling const version 
{
	return const_cast<Track *>(static_cast<const MonitorData&>(*this).getTrack(trackNr));
}

size_t MonitorData::GetNrOfSignals(TrackNr const trackNr) const
{
	return (IsValid(trackNr))
		? getTrack(trackNr)->GetNrOfSignals()
		: 0;
}

bool MonitorData::IsEmptyTrack(TrackNr const trackNr) const
{
	return (IsValid(trackNr))
		? getTrack(trackNr)->IsEmpty()
		: false;
}

bool MonitorData::AnyEmptyTracks() const
{ 
	return Apply2AllTracksB([](Track const & track){ return track.IsEmpty(); });
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
