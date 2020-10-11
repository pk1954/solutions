// MonitorData.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Track.h"
#include "MonitorData.h"

using std::distance;

void MonitorData::Initialize( Observable* const pStaticModelObservable )
{
	m_pStaticModelObservable = pStaticModelObservable;
}

void MonitorData::Reset( )
{
	for ( Track & track: m_tracks ) { track.Clear(); };
	m_tracks.clear();
	m_pStaticModelObservable->NotifyAll( true );
}

int MonitorData::GetNrOfTracks( ) const
{
	return Cast2Int(m_tracks.size());
}

bool MonitorData::NoTracks( ) const
{
	return m_tracks.size() == 0;
}

TrackIter const MonitorData::InsertTrack( TrackNr const trackNr )
{
	auto res { m_tracks.insert( m_tracks.begin() + trackNr.GetValue(), Track() ) };
	m_pStaticModelObservable->NotifyAll( true );
	return res;
}

Signal * const MonitorData::RemoveSignal( TrackNr const trackNr, SignalNr const signalNr )
{
	Signal * pSignal { nullptr };
	if ( IsValid( trackNr ) )
	{
		pSignal = getTrack( trackNr )->RemoveSignal( signalNr );
		m_pStaticModelObservable->NotifyAll( true );
	}
	return pSignal;
}

SignalNr const MonitorData::AddSignal( TrackNr const trackNr, Signal * const pSignal )
{
	SignalNr signalNr { SignalNr::NULL_VAL() };
	if ( IsValid( trackNr ) )
	{
		signalNr = getTrack( trackNr )->AddSignal( pSignal );
		m_pStaticModelObservable->NotifyAll( true );
	}
	return signalNr;
}

SignalNr const MonitorData::MoveSignal
(
	TrackNr  const trackNrOld,
	SignalNr const signalNr,
	TrackNr  const trackNrNew
)
{
	Signal * pSignal     { getTrack( trackNrOld )->RemoveSignal( signalNr ) };
	SignalNr signalNrNew { getTrack( trackNrNew )->AddSignal( pSignal ) };
	m_pStaticModelObservable->NotifyAll( true );
	return signalNrNew;
}

void MonitorData::DeleteTrack( TrackNr const trackNr )
{
	if ( IsValid( trackNr ) )
	{
		getTrack( trackNr )->Clear();
		m_tracks.erase( getTrackC( trackNr ) );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::Apply2AllTracks( TrackFunc const & func ) const
{
	for ( auto it = m_tracks.begin(); it != m_tracks.end(); ++it )
		func( it ); 
}                        

void MonitorData::Apply2AllSignals( TrackNr const trackNr, SignalFunc const & func ) const
{
	if ( IsValid( trackNr ) )
		getTrackC(trackNr)->Apply2AllSignalsC( func );
}                        

Signal const * const MonitorData::GetSignal( TrackNr const trackNr, SignalNr const signalNr ) const
{
	return IsValid( trackNr ) ? getTrackC(trackNr)->GetSignal( signalNr ) : nullptr;
}

TrackNr const MonitorData::GetTrackNr( TrackConstIter const itTrack ) const
{
	return TrackNr( Cast2Int(distance( m_tracks.begin(), itTrack )) );
}

SignalNr const MonitorData::GetSignalNr( TrackConstIter const itTrack, SignalIter const itSignal ) const
{
	return GetSignalNr( GetTrackNr( itTrack ), itSignal );
}

SignalNr const MonitorData::GetSignalNr( TrackNr const trackNr, SignalIter const itSignal ) const
{
	return IsValid( trackNr )
		? getTrackC(trackNr)->GetSignalNr( itSignal )
		: SignalNr::NULL_VAL();
}

bool const MonitorData::IsValid( TrackNr const trackNr ) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

void MonitorData::CheckTracks( ) const
{
#ifdef _DEBUG
	for ( Track const & track: m_tracks ) { track.CheckSignals(); };
#endif
}

TrackIter MonitorData::getTrack( TrackNr const trackNr )
{
	assert( IsValid( trackNr ) );
	return m_tracks.begin() + trackNr.GetValue(); 
}

TrackConstIter MonitorData::getTrackC( TrackNr const trackNr ) const 
{ 
	assert( IsValid( trackNr ) );
	return m_tracks.begin() + trackNr.GetValue(); 
}
