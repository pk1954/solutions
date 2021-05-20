// MonitorData.cpp 
//
// NNetWindows

#include "stdafx.h"
#include "Track.h"
#include "SignalFactory.h"
#include "MonitorData.h"

using std::distance;
using std::make_unique;
using std::move;

bool MonitorData::operator==( MonitorData const & rhs ) const
{
	return m_tracks == rhs.m_tracks;
}

MonitorData::MonitorData(const MonitorData& rhs)  // copy constructor
{
	for ( auto const & upTrack : rhs.m_tracks )
		m_tracks.push_back( move(make_unique<Track>(*upTrack.get())) );
}

MonitorData& MonitorData::operator=(MonitorData&& rhs) noexcept // move assignment operator
{
	if (this != &rhs)
	{
		m_tracks.clear();
		for ( auto const & upTrack : rhs.m_tracks )
			m_tracks.push_back( move(make_unique<Track>(*upTrack.get())) );
	}
	return * this;
}

void MonitorData::Reset()
{
	m_tracks.clear();
	m_idSigSelected.Set2Null();
}

int MonitorData::GetNrOfTracks() const
{
	return Cast2Int(m_tracks.size());
}

bool MonitorData::NoTracks() const
{
	return m_tracks.size() == 0;
}

void MonitorData::InsertTrack( TrackNr const trackNr )
{
	unique_ptr<Track> track = make_unique<Track>();
	m_tracks.insert( m_tracks.begin() + trackNr.GetValue(), move(track) );
}

unique_ptr<Signal> MonitorData::removeSignal( SignalId const & id )
{ 
	return getTrack(id.GetTrackNr())->RemoveSignal( id.GetSignalNr() ); 
};

SignalId const MonitorData::SetSelectedSignalId( SignalId const id )
{
	SignalId const idOld { m_idSigSelected };
	m_idSigSelected = id;
	return idOld;
}

SignalId const MonitorData::ResetSelectedSignal()
{
	SignalId const idOld { m_idSigSelected };
	m_idSigSelected.Set2Null();
	return idOld;
}

SignalNr const MonitorData::AddSignal
( 
	TrackNr      const trackNr, 
	unique_ptr<Signal> pSignal 
)
{
	return getTrack(trackNr)->AddSignal(move(pSignal));
}

unique_ptr<Signal> MonitorData::DeleteSignal( SignalId const & id )
{
	return IsValid(id.GetTrackNr())	? move(removeSignal(id)) : nullptr; 
}

SignalNr const MonitorData::AddSignal
( 
	TrackNr          const   trackNr, 
	MicroMeterCircle const & umCircle 
)
{
	return ( IsValid(trackNr) )
	? AddSignal( trackNr, move(SignalFactory::MakeSignal(umCircle)) )
	: SignalNr::NULL_VAL();
}

SignalId const MonitorData::MoveSelectedSignal(	TrackNr const trackNrNew )
{
	if ( IsValid(m_idSigSelected) && IsValid(trackNrNew) ) //-V1051
	{
		unique_ptr<Signal> pSignal     { removeSignal( m_idSigSelected ) };
		SignalNr           signalNrNew { AddSignal( trackNrNew, move(pSignal) ) };
		m_idSigSelected = SignalId( trackNrNew, signalNrNew );
	}
	return m_idSigSelected;
}

void MonitorData::DeleteTrack( TrackNr const trackNr )
{
	if ( IsValid( trackNr ) )
		m_tracks.erase( m_tracks.begin() + trackNr.GetValue() );
}

void MonitorData::Apply2AllTracks(TrackNrFunc const & func) const
{
	for (TrackNr trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
		func( trackNr ); 
}                        

void MonitorData::Apply2AllSignalsInTrack( TrackNr const trackNr, SignalNrFunc const & func ) const
{
	if ( Track const * pTrack { getTrack(trackNr) } )
		pTrack->Apply2AllSignals( func );
}                        

void MonitorData::Apply2AllSignals( SignalIdFunc const & func ) const
{
	for (TrackNr trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
	{ 
		if ( Track const * pTrack { getTrack(trackNr) } )
		{
			pTrack->Apply2AllSignals( [&](SignalNr const & signalNr){ func( SignalId(trackNr, signalNr) ); } );
		}
	}
}                        

void MonitorData::Apply2AllSignals( SignalFunc const & func ) const
{
	for (TrackNr trackNr = TrackNr(0); trackNr < TrackNr(GetNrOfTracks()); ++trackNr)
	{ 
		if ( Track const * pTrack { getTrack(trackNr) } )
		{
			pTrack->Apply2AllSignals([&](Signal const & signal)	{ func(signal); } );
		}
	}
}                        

Signal * const  MonitorData::FindSignal( SignalCrit const & crit ) const
{
	for ( unique_ptr<Track> const & upTrack: m_tracks ) 
		if ( Signal * const pSignal { upTrack->FindSignal( crit ) } )
			return pSignal;
	return nullptr;
}                        

Signal * const MonitorData::GetSignalPtr( SignalId const & id )
{
	TrackNr const trackNr { id.GetTrackNr() };
	return IsValid(trackNr) ? getTrack(trackNr)->GetSignalPtr(id.GetSignalNr()) : nullptr;
}

Signal const * const MonitorData::GetSignalPtr( SignalId const & id ) const
{
	TrackNr const trackNr { id.GetTrackNr() };
	return IsValid(trackNr) ? getTrack(trackNr)->GetSignalPtr(id.GetSignalNr()) : nullptr;
}

Signal * const MonitorData::GetSelectedSignalPtr()
{
	TrackNr const trackNr { m_idSigSelected.GetTrackNr() };
	return IsValid(trackNr)
		   ? getTrack(trackNr)->GetSignalPtr(m_idSigSelected.GetSignalNr())
		   : nullptr;
}

Signal const * const MonitorData::GetSelectedSignalPtr() const
{
	return GetSignalPtr( m_idSigSelected );
}

bool const MonitorData::IsValid( TrackNr const trackNr ) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

bool const MonitorData::IsValid( SignalId const & id ) const
{
	return IsValid(id.GetTrackNr()) && getTrack(id.GetTrackNr())->IsValid(id.GetSignalNr());
}

void MonitorData::CheckTracks() const
{
#ifdef _DEBUG
	for ( unique_ptr<Track> const & upTrack: m_tracks ) 
		upTrack->CheckSignals(); 
#endif
}

Track const * const MonitorData::getTrack(TrackNr const trackNr) const
{
	return IsValid(trackNr) ? m_tracks[trackNr.GetValue()].get() : nullptr;
}

Track * const MonitorData::getTrack(TrackNr const trackNr) // calling const version 
{
	return const_cast<Track *>(static_cast<const MonitorData&>(*this).getTrack( trackNr ));
}

bool const MonitorData::IsEmptyTrack(TrackNr const trackNr) const 
{ 
	return IsValid(trackNr) ? getTrack(trackNr)->IsEmpty() : true; 
}

Signal * const MonitorData::FindSensor( MicroMeterPnt const & umPos ) const
{
	return FindSignal( [&](Signal const & s) { return s.Includes(umPos); } );
}
