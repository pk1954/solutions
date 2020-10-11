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

void MonitorData::InsertTrack( TrackNr const trackNr )
{
	m_tracks.insert( m_tracks.begin() + trackNr.GetValue(), Track() );
	m_pStaticModelObservable->NotifyAll( true );
}

Signal * const MonitorData::DeleteSignal( SignalId const & id )
{
	Signal * pSignal { nullptr };
	if ( IsValid( id.trackNr ) )
	{
		pSignal = getTrack( id.trackNr )->DeleteSignal( id.signalNr );
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

SignalId const MonitorData::MoveSignal(	SignalId const & id, TrackNr const trackNrNew )
{
	SignalId idNew { id };
	if ( IsValid( id ) && IsValid(trackNrNew) )
	{
		Signal * pSignal     { getTrack( id.trackNr )->DeleteSignal( id.signalNr ) };
		SignalNr signalNrNew { getTrack( trackNrNew )->AddSignal( pSignal ) };
		idNew = SignalId( trackNrNew, signalNrNew );
		m_pStaticModelObservable->NotifyAll( true );
	}
	return idNew;
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
	for ( int i = 0; i < m_tracks.size(); ++i )
		func( TrackNr( i ) ); 
}                        

void MonitorData::Apply2AllSignals( TrackNr const trackNr, SignalFunc const & func ) const
{
	if ( IsValid( trackNr ) )
		getTrackC(trackNr)->Apply2AllSignals( func );
}                        

void MonitorData::Apply2AllSignals( function<void(SignalId const &)> const & func ) const
{
	Apply2AllTracks
	( 
		[&]( TrackNr const trackNr )
		{
			getTrackC(trackNr)->Apply2AllSignals
			(
				[&](SignalNr const & signalNr){	func( SignalId(trackNr, signalNr) ); }
			);
		}
	);
}                        

Signal const * const MonitorData::GetSignal( SignalId const & id ) const
{
	return IsValid( id.trackNr ) ? getTrackC(id.trackNr)->GetSignal( id.signalNr ) : nullptr;
}

bool const MonitorData::IsValid( TrackNr const trackNr ) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

bool const MonitorData::IsValid( SignalId const & id ) const
{
	return IsValid(id.trackNr) && getTrackC(id.trackNr)->IsValid(id.signalNr);
}

void MonitorData::CheckTracks( ) const
{
#ifdef _DEBUG
	for ( Track const & track: m_tracks ) 
		track.CheckSignals(); 
#endif
}

Track * const MonitorData::getTrack( TrackNr const trackNr )
{
	assert( IsValid( trackNr ) );
	return & m_tracks[trackNr.GetValue()]; 
}

vector<Track>::const_iterator const MonitorData::getTrackC( TrackNr const trackNr ) const 
{ 
	assert( IsValid( trackNr ) );
	return m_tracks.begin() + trackNr.GetValue(); 
}
