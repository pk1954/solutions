// MonitorData.cpp 
//
// NNetWindows

#include "stdafx.h"
#include "Track.h"
#include "SignalFactory.h"
#include "MonitorData.h"

using std::distance;
using std::move;

void MonitorData::Initialize
( 
	Observable      * const pStaticModelObservable,
	SignalFactory   * const pSignalFactory
)
{
	m_pStaticModelObservable = pStaticModelObservable;
	m_pSignalFactory         = pSignalFactory;
}

void MonitorData::Reset( )
{
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

unique_ptr<SignalInterface> MonitorData::removeSignal( SignalId const & id )
{ 
	return getTrack( id.GetTrackNr() ).RemoveSignal( id.GetSignalNr() ); 
};

SignalNr const MonitorData::addSignal( TrackNr const trackNr, unique_ptr<SignalInterface> pSignal )
{
	return getTrack( trackNr ).AddSignal( move(pSignal) );
}

void MonitorData::DeleteSignal( SignalId const & id )
{
	if ( IsValid( id.GetTrackNr() ) )
	{
		removeSignal( id );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::AddSignal( TrackNr const trackNr, ShapeId const idShape )
{
	if ( IsValid( trackNr ) )
	{
		unique_ptr<SingleSignal> upSignal { m_pSignalFactory->MakeSignal( idShape ) };
		SignalNr signalNr { addSignal( trackNr, move(upSignal) ) };
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::AddSignal( TrackNr const trackNr, MicroMeterCircle const & umCircle )
{
	if ( IsValid( trackNr ) )
	{
		unique_ptr<SumSignal> upSignal { m_pSignalFactory->MakeSignal( umCircle ) };
		SignalNr signalNr { addSignal( trackNr, move(upSignal) ) };
		m_pStaticModelObservable->NotifyAll( true );
	}
}

SignalId const MonitorData::MoveSignal(	SignalId const & id, TrackNr const trackNrNew )
{
	SignalId idNew { id };
	if ( IsValid(id) && IsValid(trackNrNew) ) //-V1051
	{
		unique_ptr<SignalInterface> pSignal     { removeSignal( id ) };
		SignalNr                    signalNrNew { addSignal( trackNrNew, move(pSignal) ) };
		idNew = SignalId( trackNrNew, signalNrNew );
		m_pStaticModelObservable->NotifyAll( true );
	}
	return idNew;
}

void MonitorData::DeleteTrack( TrackNr const trackNr )
{
	if ( IsValid( trackNr ) )
	{
		m_tracks.erase( m_tracks.begin() + trackNr.GetValue() );
		m_pStaticModelObservable->NotifyAll( true );
	}
}

void MonitorData::Animation( SignalId const& id, bool const bOn )
{
	if ( id.IsNotNull() )
	GetSignal( id ).Animate( bOn );
}

void MonitorData::Apply2AllTracks( TrackFunc const & func ) const
{
	for ( int i = 0; i < m_tracks.size(); ++i )
		func( TrackNr( i ) ); 
}                        

void MonitorData::Apply2AllSignalsInTrack( TrackNr const trackNr, SignalFunc const & func ) const
{
	if ( IsValid( trackNr ) )
		getTrackC(trackNr).Apply2AllSignals( func );
}                        

void MonitorData::Apply2AllSignals( function<void(SignalId const &)> const & func ) const
{
	Apply2AllTracks
	( 
		[&]( TrackNr const trackNr )
		{
			getTrackC(trackNr).Apply2AllSignals
			(
				[&](SignalNr const & signalNr){	func( SignalId(trackNr, signalNr) ); }
			);
		}
	);
}                        

SignalInterface const & MonitorData::GetSignal( SignalId const & id ) const
{
	assert( IsValid( id.GetTrackNr() ) );
	return getTrackC( id.GetTrackNr() ).GetSignal( id.GetSignalNr() );
}

bool const MonitorData::IsValid( TrackNr const trackNr ) const
{
	return (0 <= trackNr.GetValue()) && (trackNr.GetValue() < m_tracks.size());
}

bool const MonitorData::IsValid( SignalId const & id ) const
{
	return IsValid(id.GetTrackNr()) && getTrackC(id.GetTrackNr()).IsValid(id.GetSignalNr());
}

void MonitorData::CheckTracks( ) const
{
#ifdef _DEBUG
	for ( Track const & track: m_tracks ) 
		track.CheckSignals(); 
#endif
}

Track & MonitorData::getTrack( TrackNr const trackNr )
{
	assert( IsValid( trackNr ) );
	return m_tracks[trackNr.GetValue()]; 
}

Track const & MonitorData::getTrackC( TrackNr const trackNr ) const
{
	assert( IsValid( trackNr ) );
	return m_tracks[trackNr.GetValue()]; 
}
